#include "../include/threadpool.h"
#include <chrono>

#define LOCK_MUTEX std::unique_lock<std::mutex> lock(*rwLock)
#define TIME_NOW std::chrono::high_resolution_clock::now()

template <unsigned int N>
void ThreadPool<N>::runner(unsigned short id) {
	std::shared_ptr<std::mutex> mutex(rwLock);
	std::shared_ptr<bool> localStop(stop);
	std::shared_ptr<bool> deadHost(unsafeShutdown);
	threadStatusMap.insert({ id, started});
	{
		std::unique_lock<std::mutex> lock(*mutex);
		poolStartCountdown--;
		if (poolStartCountdown == 0) condition.notify_one();
	}
	while (true)
	{
		unsigned int task;
		{
			std::unique_lock<std::mutex> lock(*mutex);
			auto startTime = TIME_NOW;
			if (!*localStop && !running) threadStatusMap[id] = paused;
			if (!*localStop && priorityQueue.empty()) threadStatusMap[id] = waiting;
			condition.wait(lock, [=] {return *localStop || (running && !priorityQueue.empty()); });
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
			task = priorityQueue.top();
			priorityQueue.pop();
			threadStatusMap[id] = working;
		}
		auto startTime = TIME_NOW;
		std::this_thread::sleep_for(std::chrono::seconds(task));
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

template <unsigned int N>
void ThreadPool<N>::updateAvgQueueSize()
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

template <unsigned int N>
ThreadPool<N>::ThreadPool(bool ExitImmediatlyOnTerminate) :
	exitImmediatlyOnTerminate(ExitImmediatlyOnTerminate)
{
	for (unsigned int i = 0; i < N; i++)
	{
		threads.emplace_back(std::thread(&ThreadPool<N>::runner, this, i));
	}
	LOCK_MUTEX;
	condition.wait(lock, [this] {return poolStartCountdown == 0; });
}

template <unsigned int N>
ThreadPool<N>::~ThreadPool()
{
	if (exitImmediatlyOnTerminate)
		terminateIm();
	else
		terminate();
}

template <unsigned int N>
void ThreadPool<N>::terminate()
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

template <unsigned int N>
void ThreadPool<N>::terminateIm()
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

template <unsigned int N>
void ThreadPool<N>::pause()
{
	LOCK_MUTEX;
	running = false;
}

template <unsigned int N>
void ThreadPool<N>::unpause()
{
	LOCK_MUTEX;
	running = true;

	condition.notify_all();
}

template <unsigned int N>
void ThreadPool<N>::pauseToggle()
{
	LOCK_MUTEX;
	running = !running;
	if (running) condition.notify_all();
}

template <unsigned int N>
unsigned int ThreadPool<N>::addTask(unsigned int task)
{
	LOCK_MUTEX;
	updateAvgQueueSize();
	priorityQueue.push(task);
	size_t out = priorityQueue.size();
	if (out = 1) condition.notify_one();
	return out;
}

template <unsigned int N>
unsigned int ThreadPool<N>::removeTask()
{
	LOCK_MUTEX;
	updateAvgQueueSize();
	unsigned int out = priorityQueue.top();
	priorityQueue.pop();
	return out;
}

//monitoring
template <unsigned int N>
unsigned int ThreadPool<N>::currentQueueSize()
{
	LOCK_MUTEX;
	return priorityQueue.size();
}

template <unsigned int N>
std::unordered_map<unsigned short, typename ThreadPool<N>::threadStatusEnum> ThreadPool<N>::currentThreadStatus()
{
	LOCK_MUTEX;
	return threadStatusMap;
}

//statistics:
template <unsigned int N>
double ThreadPool<N>::avgWaitTime()
{
	LOCK_MUTEX;
	return avgWaitTimeVar / avgWaitTimeDivider;
}

template <unsigned int N>
void ThreadPool<N>::avgWaitTimeReset()
{
	LOCK_MUTEX;
	avgWaitTimeVar = 0.0;
	avgWaitTimeDivider = 0;
}

template <unsigned int N>
double ThreadPool<N>::avgQueueSize()
{
	LOCK_MUTEX;
	return avgQueueSizeVar/avgQueueSizeDivider;
}

template<unsigned int N>
void ThreadPool<N>::avgQueueSizeReset()
{
	LOCK_MUTEX;
	avgQueueSizeVar = 0.0;
	avgQueueSizeDivider = 0.0;
}

template <unsigned int N>
double ThreadPool<N>::avgTaskCompletionTime()
{
	LOCK_MUTEX;
	return avgTaskCompletionTimeVar / avgTaskCompletionTimeDivider;
}

template <unsigned int N>
void ThreadPool<N>::avgTaskCompletionTimeReset()
{
	LOCK_MUTEX;
	avgTaskCompletionTimeVar = 0.0;
	avgTaskCompletionTimeDivider = 0;
}
