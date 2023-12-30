#include <threadpool.h>

#include <chrono>

#define LOCK_MUTEX std::unique_lock<std::mutex> lock(*rwLock)
#define TIME_NOW std::chrono::high_resolution_clock::now()


void ThreadPool::runner(unsigned short id) {
	std::shared_ptr<std::mutex> mutex;
	std::shared_ptr<bool> localStop;
	std::shared_ptr<bool> deadHost;
	{
		std::unique_lock<std::mutex> lock(*rwLock);
		mutex = rwLock;
		localStop = stop;
		deadHost = unsafeShutdown;
		threadStatusMap.insert({ id, started});
		poolStartCountdown--;
		if (poolStartCountdown == 0) condition.notify_all();
	}
	while (true)
	{
		Task current_task;
		{
			std::unique_lock<std::mutex> lock(*mutex);
			auto startTime = TIME_NOW;
			if (!*localStop && !running) threadStatusMap[id] = paused;
			if (!*localStop && priorityQueue.empty()) 
			{
				threadStatusMap[id] = waiting;
				bool finished = true;
				for (auto th : threadStatusMap)
				{
					if(th.second != waiting)
						finished = false;
				}
				if (finished)
					callSubscribersOnFinish();
			}
			condition.wait(lock, [this, localStop] {return *localStop || (running && !priorityQueue.empty()); });
			auto endTime = TIME_NOW;
			if (*localStop) 
			{
				if (!*deadHost)
				{
					threadStatusMap[id] = dead;
				}
				break;
			}
			else
			{
				avgWaitTimeVar += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
				avgWaitTimeDivider++;
			}
			updateAvgQueueSize();
			current_task = priorityQueue.top();
			priorityQueue.pop();
			threadStatusMap[id] = working;
		}
		auto startTime = TIME_NOW;
		current_task.second();
		auto endTime = TIME_NOW;
		{
			std::unique_lock<std::mutex> lock(*mutex);
			if (!*localStop || !*deadHost)
			{
				avgTaskCompletionTimeVar += std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
				avgTaskCompletionTimeDivider++;
			}
		}
	}
}


void ThreadPool::updateAvgQueueSize()
{
	//get vars
	auto now = TIME_NOW;
	double queueSize = priorityQueue.size();
	double time = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeFromLastUpdate).count();
	
	//update statistics
	timeFromLastUpdate = now;
	avgQueueSizeVar += queueSize * time;
	avgQueueSizeDivider += time;
}

ThreadPool::ThreadPool(unsigned int N, bool ExitImmediatlyOnTerminate) :
	N(N), exitImmediatlyOnTerminate(ExitImmediatlyOnTerminate)
{
	for (unsigned int i = 0; i < N; i++)
	{
		threads.emplace_back(std::thread(&ThreadPool::runner, this, i));
	}
	LOCK_MUTEX;
	condition.wait(lock, [this] {return this->poolStartCountdown == 0; });
}


ThreadPool::~ThreadPool()
{
	if (exitImmediatlyOnTerminate)
		terminateIm();
	else
		terminate();
}


void ThreadPool::terminate()
{
	{
		LOCK_MUTEX;
		if (*stop) return;
		else *stop = true;
		running = true;
	}

	condition.notify_all();

	for (auto it = threads.begin(); it != threads.end(); it++)
	{
		(*it).join();
	}
	updateAvgQueueSize();
}


void ThreadPool::terminateIm()
{
	{
		LOCK_MUTEX;
		*unsafeShutdown = true;
		if (*stop) return;
		else *stop = true;
	}

	condition.notify_all();

	for (auto it = threads.begin(); it != threads.end(); it++)
	{
		(*it).detach();
	}
	updateAvgQueueSize();
}


void ThreadPool::pause()
{
	LOCK_MUTEX;
	running = false;
}


void ThreadPool::unpause()
{
	LOCK_MUTEX;
	running = true;

	condition.notify_all();
}


void ThreadPool::pauseToggle()
{
	LOCK_MUTEX;
	running = !running;
	if (running) condition.notify_all();
}


void ThreadPool::addTask(Task task)
{
	LOCK_MUTEX;
	updateAvgQueueSize();
	priorityQueue.push(task);
	size_t out = priorityQueue.size();
	if (out == 1) 
	{
		condition.notify_one();
	}
}


void ThreadPool::removeTask()
{
	LOCK_MUTEX;
	updateAvgQueueSize();
	priorityQueue.pop();
}

//monitoring

unsigned int ThreadPool::currentQueueSize()
{
	LOCK_MUTEX;
	return priorityQueue.size();
}


std::unordered_map<unsigned short, typename ThreadPool::threadStatusEnum> ThreadPool::currentThreadStatus()
{
	LOCK_MUTEX;
	return threadStatusMap;
}

//statistics:

double ThreadPool::avgWaitTime()
{
	LOCK_MUTEX;
	return avgWaitTimeVar / avgWaitTimeDivider;
}


void ThreadPool::avgWaitTimeReset()
{
	LOCK_MUTEX;
	avgWaitTimeVar = 0.0;
	avgWaitTimeDivider = 0;
}


double ThreadPool::avgQueueSize()
{
	LOCK_MUTEX;
	return avgQueueSizeVar/avgQueueSizeDivider;
}

void ThreadPool::avgQueueSizeReset()
{
	LOCK_MUTEX;
	avgQueueSizeVar = 0.0;
	avgQueueSizeDivider = 0.0;
}


double ThreadPool::avgTaskCompletionTime()
{
	LOCK_MUTEX;
	return avgTaskCompletionTimeVar / avgTaskCompletionTimeDivider;
}


void ThreadPool::avgTaskCompletionTimeReset()
{
	LOCK_MUTEX;
	avgTaskCompletionTimeVar = 0.0;
	avgTaskCompletionTimeDivider = 0;
}

std::pair<std::set<std::function<void ()>>::iterator, bool> ThreadPool::subscribeOnFinish(std::function<void()> callback)
{
	LOCK_MUTEX;
	return subscribersOnFinish.insert(callback);
}

void ThreadPool::unsubscribeOnFinish(std::set<std::function<void()>>::iterator itterator)
{
	LOCK_MUTEX;
	subscribersOnFinish.erase(itterator);
}


void ThreadPool::callSubscribersOnFinish()
{
	for(auto item : subscribersOnFinish)
	{
		item();
	}
}