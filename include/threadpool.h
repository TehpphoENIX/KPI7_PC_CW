#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::pair<int, std::function<const int()>> Task;  

class ThreadPool
{
public:
	enum threadStatusEnum
	{
		started,
		dead,
		waiting,
		working,
		paused
	};
private:
	const unsigned int N;
	std::vector<std::thread> threads;
	bool running = true;
	std::shared_ptr<bool> stop = std::make_shared<bool>(false);
	const bool exitImmediatlyOnTerminate;

	unsigned int poolStartCountdown = N;
	std::shared_ptr<bool> unsafeShutdown = std::make_shared<bool>(false);

	static constexpr auto task_compare = [](Task a,Task b) { return (a.first > b.first); };
	std::priority_queue<Task, std::vector<Task>, decltype(task_compare)> priorityQueue;
	std::shared_ptr<std::mutex> rwLock = std::make_shared<std::mutex>();
	std::condition_variable condition;

	double avgWaitTimeVar = 0.0;
	unsigned int avgWaitTimeDivider = 0;

	double avgTaskCompletionTimeVar = 0.0;
	unsigned int avgTaskCompletionTimeDivider = 0;

	double avgQueueSizeVar = 0.0;
	double avgQueueSizeDivider = 0.0;
	std::chrono::system_clock::time_point timeFromLastUpdate = std::chrono::high_resolution_clock::now();
	std::unordered_map<unsigned short, threadStatusEnum> threadStatusMap;

	static constexpr auto sub_compare = [](std::function<void()> a,std::function<void()> b) { return true; };
	std::set<std::function<void()>, decltype(sub_compare)> subscribersOnFinish;

public:
	ThreadPool(unsigned int N, bool exitImmediatlyOnTerminate = false);

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&& other) = delete;
	ThreadPool& operator=(ThreadPool& rhs) = delete;
	ThreadPool& operator=(ThreadPool&& rhs) = delete;
	~ThreadPool();

	void terminate();
	void terminateIm();

	void pause();
	void unpause();
	void pauseToggle();

	void addTask(Task Task);
	void removeTask();

	//monitoring
	unsigned int currentQueueSize();
	std::unordered_map<unsigned short, ThreadPool::threadStatusEnum> currentThreadStatus();
	static const char* toString(ThreadPool::threadStatusEnum v)
	{
		switch (v)
		{
		case started:   return "started";
		case dead:   return "dead";
		case waiting: return "waiting";
		case working: return "working";
		case paused: return "paused";
		default:      return "???";
		}
	}

	//statistics
	unsigned int numberOfThreds() { return N; }
	double avgWaitTime();
	void avgWaitTimeReset();
	double avgQueueSize();
	void avgQueueSizeReset();
	double avgTaskCompletionTime();
	void avgTaskCompletionTimeReset();

	//subscribtions
	std::pair<std::set<std::function<void ()>>::iterator, bool> subscribeOnFinish(std::function<void()> callback);
	void unsubscribeOnFinish(std::set<std::function<void ()>>::iterator itterator);
private:
	void runner(unsigned short id);

	//must be called before queue operations
	void updateAvgQueueSize();
	void callSubscribersOnFinish();
};