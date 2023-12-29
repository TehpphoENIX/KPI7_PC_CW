#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <assert.h>

#include <thread_pool.h>
#include <inverted_index.h>

const int HandleFile(const std::filesystem::path p, ThreadPool& tp, InvertedIndex& ii)
{
    
    if (std::filesystem::is_directory(p))
    {
        std::filesystem::directory_iterator diritt(p);
        for(auto file : diritt)
        {
            tp.addTask({1,std::bind(HandleFile,file, tp, ii)});
        }
    }
    else if (std::filesystem::is_regular_file(p))
    {
        //TODO
    }

    return 0;
}

int main(int argc, char** argv) {
    //transfer argv to better container
    std::vector<std::string> argvec;
    for (int i = 1; i < argc; i++)
    {
        argvec.push_back(std::string(argv[i]));
    }

    //get filepathes
    std::vector<std::filesystem::path> initialFileList;
    for (auto arg : argvec)
    {
        if(std::filesystem::exists(arg))
        {
            initialFileList.push_back(arg);
        }
    }

    //if empty, add current dir
    if (initialFileList.size() == 0)
    {
        initialFileList = { "." };
    }

    ThreadPool tp(4);
    InvertedIndex invIn(64ul);
    //Construction process
    {
        std::mutex m;
        std::condition_variable c;
        std::atomic_bool constructionFinished = false;
        auto subIttPair = tp.subscribeOnEmpty([&constructionFinished, &c] { 
                bool expected = false;
                do
                {
                    constructionFinished.compare_exchange_strong(expected, true);
                }
                while (expected == false);
                c.notify_one();
            });
        assert(("Subscribtion Successful", subIttPair.second));
        {
            for (auto file : initialFileList)
            {
                tp.addTask({1,std::bind(HandleFile,file, tp, invIn)});
            }
            std::unique_lock<std::mutex> lock;
            c.wait(lock,[&constructionFinished]{ return static_cast<bool>(constructionFinished); });
        }
        tp.unsubscribeOnEmpty(subIttPair.first);
    }

    
}