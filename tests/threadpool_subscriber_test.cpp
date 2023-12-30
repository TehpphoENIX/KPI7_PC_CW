#include <threadpool.h>

#include <atomic>
#include <cassert>


int threadpool_subscriber_test(int argc, char** argv)
{
    bool subscriber_called1 = false;
    bool subscriber_called2 = false;
    bool subscriber_called3 = false;
    std::vector<bool> tasks;
	for (int i = 0; i < 10; i++)
	{
		tasks.push_back(false);
	}
	

	ThreadPool tp(4);
	tp.subscribeOnFinish([&subscriber_called1]{
		subscriber_called1 = true;
	});
	tp.subscribeOnFinish([&subscriber_called2]{
		subscriber_called2 = true;
	});
	auto dont_call = tp.subscribeOnFinish([&subscriber_called3]{
		subscriber_called3 = true;
	});
	tp.unsubscribeOnFinish(dont_call.first);
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
	assert(("all subscribers called", subscriber_called1 && subscriber_called2));
	assert(("all unsubs dont called", !subscriber_called3));
    return 0;
}