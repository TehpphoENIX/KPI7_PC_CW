#include "../include/thread_pool.h"
// #include <atomic>
// #include <cassert>

int tests_thread_pool_completion_test(int argc, char** argv)
{
    
    // std::vector<bool> tasks(false, 10);

	ThreadPool<4> tp(false);
	// for (int i=0; i < tasks.size(); i++)
	// {
	// 	tp.addTask({0,std::function<const int()>([&tasks,i]{ tasks[i] = true; return 0; })});
	// }
	// while (tp.currentQueueSize() != 0)
	// {
	// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	// }

	// assert(("queue must be empty", 0u == tp.currentQueueSize()));
    return 0;
}