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

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#include <thread_pool.h>
#include <inverted_index.h>

std::string server_ip = "127.0.0.1",
    server_port = "3000";
int server_queue = 1024;

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

        //Addr discovery
        addrinfo hints = {}, *list, *curr;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;
        if(getaddrinfo("127.0.0.1", "3000", &hints, &list))
        {
            std::cerr << "getaddrinfo error: " << strerror(errno) << std::endl;
            return 1;
        }

        int serverSocket = -1;
        for (curr = list; curr != NULL; curr = curr->ai_next) {
            int  reuse = 1;

            serverSocket = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol);
            if (serverSocket == -1)
                continue;

            if (bind(serverSocket, curr->ai_addr, curr->ai_addrlen) == -1) {
                close(serverSocket);
                serverSocket = -1;
                continue;
            }

            if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
                            &reuse, sizeof (int)) == -1) {
                close(serverSocket);
                serverSocket = -1;
                continue;
            }

            if (listen(serverSocket, server_queue) == -1) {
                close(serverSocket);
                serverSocket = -1;
                continue;
            }

            break;
        }
        freeaddrinfo(list);
        if (serverSocket == -1) {
            std::cerr << "Server start failed. No valid addr.";
            return -1;
        }
        
        std::cout << "Server started!" << std::endl;

        sockaddr clientSockAddr;
        socklen_t clientSockAddrLen;
        int clientSocket = -1;
        while(clientSocket = accept(serverSocket, &clientSockAddr, &clientSockAddrLen))
        {
            tp.addTask({0,[clientSocket, clientSockAddr, clientSockAddrLen, &invIn]-> const int{
                std::string host, port, clientId;
                host.resize(NI_MAXHOST);
                port.resize(NI_MAXSERV);
                getnameinfo(&clientSockAddr, clientSockAddrLen, host.data(), host.length(), port.data(), port.length(), NI_NUMERICSERV | NI_NUMERICHOST);
                clientId = host + ":" + port;
                std::stringstream message("<>");
                message << clientId << " connected\n";
                std::cout << message.str();
                message.str("<>");

                std::string buffer(4096, '\0');
                recv(clientSocket, buffer.data(), buffer.length(), 0);
                message << clientId << "sent:\n";
                message << buffer << "\n";
                std::cout << message.str();
                message.str("<>");

                std::stringstream ss(buffer);
                std::string word;
                std::set<std::string> listOfDocuments;
                while (ss >> word)
                {
                    if (invIn.find(word))
                    {
                        std::vector<std::string> documents = invIn.read(word);

                        for (auto doc : documents)
                        {
                            if ()
                        }
                    }
                }
                

                return 0;
            }});
        }

        close(serverSocket);
        return 0;
    }
    catch(std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}