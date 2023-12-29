#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <iostream>
#include <set>
#include <fstream>
#include <sstream>

#include <thread_pool.h>
#include <inverted_index.h>

const int HandleRegularFile(const std::filesystem::path p, InvertedIndex& ii)
{
    std::set<std::string> words;
    std::ifstream file(p);

    std::string word;
    while (file >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.insert(word);
    }
    
    std::vector<std::pair<std::string,std::string>> insertion;

    for (auto word : words)
    {
        insertion.push_back({word, std::filesystem::absolute(p)});
    }

    ii.insertBatch(insertion);
    return 0;
}

const int HandleFile(const std::filesystem::path p, ThreadPool& tp, InvertedIndex& ii)
{
    
    if (std::filesystem::is_directory(p))
    {
        std::filesystem::directory_iterator diritt(p);
        for(auto file : diritt)
        {
            tp.addTask({1,std::function<const int()>([file, &tp, &ii] -> const int { return HandleFile(file, tp, ii); })});
        }
    }
    else if (std::filesystem::is_regular_file(p))
    {
        tp.addTask({0,[p, &ii]{ return HandleRegularFile(p, ii); }});
    }

    return 0;
}

int main(int argc, char** argv) 
{
    try
    {//transfer argv to better container
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
    std::cout << "constructing..." << std::endl;
    {
        std::mutex m;
        std::condition_variable c;
        auto subIttPair = tp.subscribeOnFinish([&c] { 
                c.notify_one();
            });
        assert(("Subscribtion Successful", subIttPair.second));
        {
            for (auto file : initialFileList)
            {
                tp.addTask({1, [file, &tp, &invIn]{ return HandleFile(file, tp, invIn); }});
            }
            std::unique_lock<std::mutex> lock(m);
            c.wait(lock);
        }
        tp.unsubscribeOnFinish(subIttPair.first);
    }
    invIn.finish();
    std::cout << "construction completed\n\n" << std::endl;
    //Server start
        return 0;
    }
    catch(std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}