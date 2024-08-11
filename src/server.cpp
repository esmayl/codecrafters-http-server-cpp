#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include <sys/types.h>

#include "WebServer.h"

// argv == parameter
// argc == amount of character executable call
int main(int argc, char **argv)
{
    WebServer webServer(4221,4096);

    int i;
    for (i = 0; i < argc; i++)
    {
        // printf("%s\n",argv[i]);

        // Setup directory for using with FileController endpoint
        if(strcmp(argv[i],"--directory") == 0 && i + 1 < argc)
        {

            if(std::filesystem::exists(argv[i+1]))
            {
                printf("Setup directory: %s\n", argv[i+1]);
                webServer.SetupDirectory(argv[i+1]);
            }
            else
            {
                printf("Directory '%s\n' does not exist", argv[i+1]);
                return 1;
            }
            break;
        }
    }

    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here!\n";

#ifdef _WIN64
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"Error at WSAStartup()\n");
        return 1;
    }
#endif

    SocketWrapper serverSocketWrapper;

    // Uncomment this block to pass the first stage
    serverSocketWrapper.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketWrapper.socket == 0)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors

    serverSocketWrapper.reuse = 1;

    if (setsockopt(serverSocketWrapper.socket, SOL_SOCKET, SO_REUSEADDR,&serverSocketWrapper.reuse , sizeof(serverSocketWrapper.reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(webServer.port);

    if (bind(serverSocketWrapper.socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port " << webServer.port << "\n" << std::endl;
        return 1;
    }

    int max_connection_backlog = 5;
    if (listen(serverSocketWrapper.socket, max_connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
    }

    // u_long mode =1;
    // ioctlsocket(ListenSocket,FIONBIO,&mode); // non - blocking

    std::cout << "Waiting for a client to connect...\n";

    std::vector<SocketWrapper> clients;

    while(true)
    {
        clients.push_back(WebServer::AcceptConnection(serverSocketWrapper));

        webServer.HandleRequest(clients.back());

        clients.pop_back();
    }

    #ifdef _WIN64
        closesocket(serverSocketWrapper.socket);

        WSACleanup();
    #else
        close(serverSocketWrapper.socket);
    #endif

    return 0;
}
