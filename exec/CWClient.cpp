#include <iostream>
#include <string>

#include <getopt.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const char* shortopts = "hi:p:";
const option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"ip", required_argument, NULL, 'i'},
    {"port", required_argument, NULL, 'p'},
    {0}
};
std::string serviceAddr = "127.0.0.1";
int servicePort = 3000;
std::string request = "";
const std::string help = 
"\
usage:\n\
CWClient [OPTION]... [FILE]...\n\
\n\
Send request string to service.\n\
\n\
options:\n\
-i, --ip\n\
    service ip address (localhost by default).\n\
-p, --port\n\
    service port (3000 by default).\n\
-h, --help\n\
    show help and exit.\n\
";

int main(int argc, char** argv)
{
    //arg handling
    int option;
    bool optParseErr = false;
    while((option = getopt_long(argc, argv, shortopts, longopts, 0)) != -1)
    {
        
        switch (option)
        {
        case 'i':
            serviceAddr = (optarg[0] == '=')? optarg + 1 : optarg;
            break;
        case 'p':
            servicePort = std::stoi((optarg[0] == '=')? optarg + 1 : optarg);
            break;
        case 'h':
            std::cout << help << std::endl;
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

    for (int i = optind; i < argc; i++)
    {
        request += argv[i];
        request += " ";
    }

    //address resolution
    addrinfo hints = {}, *addrList, *addrCurrent;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    if(getaddrinfo("127.0.0.1", 0, &hints, &addrList) != 0)
    {
        std::cerr << "getaddrinfo error: " << strerror(errno) << std::endl;
        return 1;
    }
    int clientSocket = -1;
    for (addrCurrent = addrList; addrCurrent != NULL; addrCurrent = addrCurrent->ai_next) {
        int  reuse = 1;
        clientSocket = socket(addrCurrent->ai_family, addrCurrent->ai_socktype, addrCurrent->ai_protocol);
        if (clientSocket == -1)
            continue;
        if (bind(clientSocket, addrCurrent->ai_addr, addrCurrent->ai_addrlen) == -1) {
            close(clientSocket);
            clientSocket = -1;
            continue;
        }
        if (setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR,
                        &reuse, sizeof (int)) == -1) {
            close(clientSocket);
            clientSocket = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(addrList);
    if (clientSocket == -1) {
        std::cerr << "Server start failed. No valid address available.";
        return -1;
    }
    
    //connecting to server
    sockaddr_in serviceSockAddr = {};
    serviceSockAddr.sin_family = AF_INET;
    serviceSockAddr.sin_addr.s_addr = inet_addr(serviceAddr.c_str());
    serviceSockAddr.sin_port = htons(servicePort);
    if(connect(clientSocket, reinterpret_cast<sockaddr*>(&serviceSockAddr), sizeof(serviceSockAddr)) == -1)
    {
        std::cerr << "Connection error: " << strerror(errno) << std::endl;
        return 1;
    }
    
    //send request
    std::string buffer = request;
    if(send(clientSocket, buffer.c_str(), buffer.size(), 0) == -1)
    {
        std::cerr << "send error: " << strerror(errno) << std::endl;
        return 1;
    }

    //recv response
    buffer.clear();
    buffer.resize(4096);
    if (recv(clientSocket, buffer.data(), buffer.size(), 0) == -1)
    {
        std::cerr << "recv error: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << buffer << std::endl;
}