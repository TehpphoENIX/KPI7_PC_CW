#include <stdlib.h>

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>

int main(int argc, char** argv)
{
    if(argc != 3) return 1;
    const int c_of_threads = std::stoi(argv[1]);
    const std::string request = argv[2];

    std::mutex m;
    std::condition_variable c;
    std::vector<std::thread> ths;
    int countdown = c_of_threads;
    for(int i = 0; i < c_of_threads; i++)
    {
        ths.push_back(std::thread([i, &c, &m, &request, &countdown]{
            {
                std::unique_lock<std::mutex> lck(m);
                countdown--;
                if (countdown > 0)
                {
                    c.wait(lck);
                }
                else
                {
                    c.notify_all();
                }
            }
            system((std::string("echo \"") + request + "\" | socat -t 60 tcp:localhost:3000 -").c_str());
        }));
    }
    c.notify_all();
    for(auto &th : ths)
    {
        th.join();
    }
}