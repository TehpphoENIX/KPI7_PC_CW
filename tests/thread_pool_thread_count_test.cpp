#include <atomic>
#include <cassert>

#include <thread_pool.h>

int thread_pool_thread_count_test(int argc, char** argv)
{
	for (int N = 1; N < 50; N++)
	{
		ThreadPool tp(N);

		assert(("Number of threads is exactly as it should be (N=%s)", N, tp.currentThreadStatus().size() == N));
    	return 0;
	}
}