#include <assert.h>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <mutex>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <getopt.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <invertedindex.h>
#include <threadpool.h>
#include <signal.h>

const char* shortopts = "hvj:i:p:q:b";
const option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"threads", required_argument, NULL, 'j'},
    {"ip", required_argument, NULL, 'i'},
    {"port", required_argument, NULL, 'p'},
    {"queue", required_argument, NULL, 'q'},
    {"build-only", no_argument, NULL, 'b'},
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
-i, --ip [ip]\n\
    server ip address (localhost by default).\n\
-p, --port [portnum]\n\
    server port (3000 by default).\n\
-q, --queue [size]\n\
    server queue size (1024 by default).\n\
-b, --build-only\n\
    build inverted index and exit.\n\
-h, --help\n\
    show help and exit.\n\
-v, --version\n\
    show version and exit.\n\
";
std::string serverIp = "127.0.0.1";
std::string serverPort = "3000";
int serverQueue = 1024;
int threadCount = 4;
bool buildOnly = false;
int serverSocket;
void handleSignal(int signum) 
{
    close(serverSocket);
    exit(EXIT_SUCCESS);
}

const int HandleRegularFile(const std::filesystem::path filePath, InvertedIndex& invIn)
{
    std::set<std::string> words;
    std::ifstream file(filePath);

    std::string word;
    while (file >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.insert(word);
    }
    
    std::vector<std::pair<std::string,std::string>> insertion;

    for (auto word : words)
    {
        insertion.push_back({word, std::filesystem::absolute(filePath)});
    }

    invIn.insertBatch(insertion);
    return 0;
}

const int HandleFile(const std::filesystem::path filePath, ThreadPool& threadPool, InvertedIndex& invIn)
{
    
    if (std::filesystem::is_directory(filePath))
    {
        std::filesystem::directory_iterator diritt(filePath);
        for(auto file : diritt)
        {
            threadPool.addTask({1,std::function<const int()>([file, &threadPool, &invIn] -> const int { return HandleFile(file, threadPool, invIn); })});
        }
    }
    else if (std::filesystem::is_regular_file(filePath))
    {
        threadPool.addTask({0,[filePath, &invIn]{ return HandleRegularFile(filePath, invIn); }});
    }

    return 0;
}

int main(int argc, char** argv) 
{
    try
    {
        int option;
        bool optParseErr = false;
        while((option = getopt_long(argc, argv, shortopts, longopts, 0)) != -1)
        {
            switch (option)
            {

            case 'j':
                threadCount = std::stoi((optarg[0] == '=')? optarg + 1 : optarg);
                break;
            case 'i':
                serverIp = (optarg[0] == '=')? optarg + 1 : optarg;
                break;
            case 'p':
                serverPort = (optarg[0] == '=')? optarg + 1 : optarg;
                break;
            case 'q':
                serverQueue = std::stoi((optarg[0] == '=')? optarg + 1 : optarg);
                break;
            case 'b':
                buildOnly = true;
                break;
            case 'h':
                std::cout << help << std::endl;
                return 0;
            case 'v':
                std::cout << version << std::endl;
                return 0;
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
        std::vector<std::filesystem::path> initialFilesList;
        for (int i = optind; i < argc; i++)
        {
            if(std::filesystem::exists(argv[i]))
            {
                initialFilesList.push_back(argv[i]);
            }
        }

        //if empty, add current dir
        if (initialFilesList.size() == 0)
        {
            initialFilesList = { "." };
        }

        std::cout << "Ammount of worker threads is set to " << threadCount << std::endl;
        ThreadPool threadPool(threadCount);
        InvertedIndex invIn(64ul);

        //Construction process
        std::cout << "constructing..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::mutex m;
            std::condition_variable finishCondition;
            auto subIttPair = threadPool.subscribeOnFinish([&finishCondition] { 
                    finishCondition.notify_one();
                });
            assert(("Subscribtion Successful", subIttPair.second));
            {
                for (auto file : initialFilesList)
                {
                    threadPool.addTask({1, [file, &threadPool, &invIn]{ return HandleFile(file, threadPool, invIn); }});
                }
                std::unique_lock<std::mutex> lock(m);
                finishCondition.wait(lock);
            }
            threadPool.unsubscribeOnFinish(subIttPair.first);
        }
        invIn.finish();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "construction completed in " << end - start << "\n" << std::endl;
        if(buildOnly)
            return 0;

        //Server start

        //Addr discovery
        addrinfo hints = {}, *addrList, *addrCurrent;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;
        if(getaddrinfo(serverIp.c_str(), serverPort.c_str(), &hints, &addrList) != 0)
        {
            std::cerr << "getaddrinfo error: " << strerror(errno) << std::endl;
            return 1;
        }

        serverSocket = -1;
        for (addrCurrent = addrList; addrCurrent != NULL; addrCurrent = addrCurrent->ai_next) {
            int  reuse = 1;

            serverSocket = socket(addrCurrent->ai_family, addrCurrent->ai_socktype, addrCurrent->ai_protocol);
            if (serverSocket == -1)
                continue;

            if (bind(serverSocket, addrCurrent->ai_addr, addrCurrent->ai_addrlen) == -1) {
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

            if (listen(serverSocket, serverQueue) == -1) {
                close(serverSocket);
                serverSocket = -1;
                continue;
            }

            break;
        }
        freeaddrinfo(addrList);
        if (serverSocket == -1) {
            std::cerr << "Server start failed. No valid address available." << std::endl;
            return -1;
        }
        else
        {
            signal(SIGINT, handleSignal);
            signal(SIGTERM, handleSignal);
            signal(SIGHUP, handleSignal);
            signal(SIGKILL, handleSignal);
        }
        
        std::cout << "Server started on " << serverIp << ':' << serverPort << std::endl;

        sockaddr clientSockAddr;
        socklen_t clientSockAddrLen;
        int clientSocket = -1;
        while(clientSocket = accept(serverSocket, &clientSockAddr, &clientSockAddrLen))
        {
            if(clientSocket == -1)
            {
                std::cerr << "accept error: " << strerror(errno) << std::endl;
            }
            else
            { 
                threadPool.addTask({0,[clientSocket, clientSockAddr, clientSockAddrLen, &invIn]-> const int{
                    //get client ip and port
                    std::string host, port, clientId;
                    host.resize(NI_MAXHOST);
                    port.resize(NI_MAXSERV);
                    getnameinfo(&clientSockAddr, clientSockAddrLen, host.data(), host.length(), port.data(), port.length(), NI_NUMERICSERV | NI_NUMERICHOST);
                    clientId = host + ":" + port;
                    std::stringstream logMessage;
                    logMessage << "<" << std::this_thread::get_id() << "> ";
                    logMessage << clientId << " connected\n";
                    std::cout << logMessage.str();

                    //get client message
                    std::string buffer(4096, '\0');
                    if (recv(clientSocket, buffer.data(), buffer.length(), 0) == -1)
                    {
                        logMessage.str("");
                        logMessage << "<" << std::this_thread::get_id() << "> ";
                        logMessage << "send error: " << strerror(errno) << "\n";
                        std::cout << logMessage.str();
                        return 1;
                    }
                    buffer = buffer.c_str();
                    logMessage.str("");
                    logMessage << "<" << std::this_thread::get_id() << "> ";
                    logMessage << clientId << " sent:\n";
                    logMessage << buffer << "\n";
                    std::cout << logMessage.str();

                    //handle request
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
                    for (auto doc : listOfDocuments)
                    {
                        buffer += doc + '\n';
                    }
                    if(buffer.empty())
                    {
                        buffer += '\n';
                    }

                    //send response
                    if (send(clientSocket, buffer.c_str(), buffer.size(), 0) == -1)
                    {
                        logMessage.str("");
                        logMessage << "<" << std::this_thread::get_id() << "> ";
                        logMessage << "send error: " << strerror(errno) << "\n";
                        std::cout << logMessage.str();
                        return 1;
                    }
                    logMessage.str("");
                    logMessage << "<" << std::this_thread::get_id() << "> ";
                    logMessage << "to " << clientId << ":\n";
                    logMessage << buffer << "\n";
                    std::cout << logMessage.str();

                    close(clientSocket);
                    return 0;
                }}); 
            }
        }
    }
    catch(std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}