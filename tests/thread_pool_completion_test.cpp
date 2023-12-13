#include "../include/thread_pool.h"
#include <atomic>
#include <cassert>

int thread_pool_completion_test(int argc, char** argv)
{
    
    std::vector<bool> tasks;
	for (int i = 0; i < 10; i++)
	{
		tasks.push_back(false);
	}
	

	ThreadPool tp(4);
	for (int i=0; i < tasks.size(); i++)
	{
		tp.addTask({0,std::function<const int()>([&tasks,i]{ tasks[i] = true; return 0; })});
	}
	int i = 0;
	while (tp.currentQueueSize() != 0 && i < 10)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		i++;
	}

	assert(("queue must be empty", 0u == tp.currentQueueSize()));
	bool fail = false;
	for(auto item : tasks)
	{
		if(!item)
		{
			fail == true;
			break;
		}
	}
	assert(("all tasks completed", !fail));
    return 0;
}