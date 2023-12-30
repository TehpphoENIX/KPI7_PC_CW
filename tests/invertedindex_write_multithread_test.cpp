#include <invertedindex.h>

#include <assert.h>
#include <vector>
#include <string>
#include <thread>

int invertedindex_write_multithread_test(int argc, char** argv)
{
    const int ammount_of_threads = 8;
    InvertedIndex ii;
    std::vector<std::vector<std::pair<std::string, std::string>>> threads_pairs;

    for (int i = 0; i < ammount_of_threads; i++)
    {
        std::vector<std::pair<std::string, std::string>> pairs;
        for (int j = 0; j < 10; j++)
        {
            pairs.push_back({std::to_string(i*100+j), std::to_string(-100*i-j)});
        }
        threads_pairs.push_back(pairs);
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < ammount_of_threads; i++)
    {
        
        threads.push_back(std::thread([i, &ii, &threads_pairs]{
            for(auto pair : threads_pairs[i])
            {
                ii.insert(pair.first, pair.second);
            }
        }));
    }
    
    for (auto &thread : threads)
    {
        thread.join();
    }

    ii.finish();

    for (auto pairs : threads_pairs)
    {
        for (auto pair : pairs)
        {
            assert(("element found", ii.find(pair.first) ));

            assert(("element retrievable", *ii.read(pair.first).begin() == pair.second));
        }
    }
    return 0;
}