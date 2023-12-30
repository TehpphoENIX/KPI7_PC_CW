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

#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#include <thread_pool.h>
#include <inverted_index.h>


const char* shortopts = "hj:i:p:q:";
const option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"threads", required_argument, NULL, 'j'},
    {"ip", required_argument, NULL, 'i'},
    {"port", required_argument, NULL, 'p'},
    {"queue", required_argument, NULL, 'q'},
    {0}
};
const std::string version = "0.1.0";
const std::string help = 
"\
usage:\n\
CWServer [OPTION]... [FILE]...\n\
\n\
Construct inverted index of FILEs and then provide service over TCP/IP that\n\
recieves index search prompt and returns search result.\n\
\n\
options:\n\
-j, --threads\n\
    ammount of threads to use (4 by default).\n\
-i, --ip\n\
    server ip address (localhost by default).\n\
-p, --port\n\
    server port (3000 by default).\n\
-q, --queue\n\
    server queue size (1024 by default).\n\
-h, --help\n\
    show help and exit.\n\
-v, --version\n\
    show version and exit.\n\
";
std::string server_ip = "127.0.0.1";
std::string server_port = "3000";
int server_queue = 1024;
int threadCount = 4;

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
    {
        int p;
        bool optParseErr = false;
        while((p = getopt_long(argc, argv, shortopts, longopts, 0)) != -1)
        {
            switch (p)
            {

            case 'j':
                threadCount = std::stoi(optarg + 1);
                break;
            case 'i':
                server_ip = optarg + 1;
                break;
            case 'p':
                server_port = optarg + 1;
                break;
            case 'q':
                server_queue = std::stoi(optarg + 1);
                break;
            case 'h':
                std::cout << help << std::endl;
                return 0;
            case 'v':
                std::cout << "version" << std::endl;
            default:
                optParseErr = true;
                break;
            }
        }

        if (optParseErr)
        {
            return 1;
        }

        //get filepathes
        std::vector<std::filesystem::path> initialFileList;
        for (int i = optind; i < argc; i++)
        {
            if(std::filesystem::exists(argv[i]))
            {
                initialFileList.push_back(argv[i]);
            }
        }

        //if empty, add current dir
        if (initialFileList.size() == 0)
        {
            initialFileList = { "." };
        }

        std::cout << "Ammount of worker threads is set to " << threadCount << std::endl;
        ThreadPool tp(threadCount);
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
        if(getaddrinfo(server_ip.c_str(), server_port.c_str(), &hints, &list) != 0)
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
        
        std::cout << "Server started on " << server_ip << ':' << server_port << std::endl;

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
                message << clientId << " sent:\n";
                message << buffer << "\n";
                std::cout << message.str();
                message.str("<>");

                std::stringstream ss(buffer);
                std::string word;
                std::set<std::string> listOfDocuments;
                bool notFirst = false;
                while (ss >> word)
                {
                    if (invIn.find(word))
                    {
                        std::set<std::string> documents = invIn.read(word);
                        if(notFirst){
                            std::vector<std::string> toErase;
                            for (auto doc : listOfDocuments)
                            {
                                if(documents.find(doc) == documents.end())
                                    toErase.push_back(doc);
                            }
                            for (auto doc : toErase)
                            {
                                listOfDocuments.erase(doc);
                            }
                        }
                        else
                        {
                            listOfDocuments = documents;
                            notFirst = true;
                        }
                    }
                }
                buffer = std::string();
                for(auto doc : listOfDocuments)
                {
                    buffer += doc + '\n';
                }

                send(clientSocket, buffer.c_str(), buffer.size(), 0);
                message << "to " << clientId << ":\n";
                message << buffer << "\n";
                std::cout << message.str();

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