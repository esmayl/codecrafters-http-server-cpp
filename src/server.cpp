#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/types.h>

#include "HttpPacket.h"
#include "Globals.h"
#include "controllers/EchoController.h"
#include "controllers/UserAgentController.h"

#ifdef _WIN64

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <cstdio>

#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

int port = 4221;

HttpPacket ParseRequestHeader(const std::string & rawString);

HttpPacket ParseRequestHeader(const std::string &rawString)
{
    return HttpPacket(rawString);
}

char* CreateEmptyResponse(const char* response)
{
    size_t responseLength = strlen(response);
    char* tempCharPointer = new char[responseLength+2];

    strcpy(tempCharPointer,response);

    tempCharPointer[responseLength] = '\r';
    tempCharPointer[responseLength+1] = '\n';

    return tempCharPointer;
}

SocketWrapper AcceptConnection(SOCKET connectedClient)
{
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    SocketWrapper returnSocket;
    returnSocket.socket = accept(connectedClient, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);

    return returnSocket;
}

void HandleRequest(SocketWrapper connectedClient)
{
    char receiveBuffer[1024];

    ssize_t readBytes = 0;
    std::string s;

    if((readBytes = recv(connectedClient.socket,receiveBuffer,sizeof receiveBuffer - 1 ,0)) > 0)
    {
        receiveBuffer[readBytes] = '\0';
        s.append(receiveBuffer,readBytes);

        HttpPacket resp = ParseRequestHeader(s);

        std::cout << "Received: " << resp.GetEndpoint() << std::endl;
        if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().compare(0,10,"user-agent") == 0)
        {
            UserAgentController::SendResponse(connectedClient,resp);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().compare(0,4,"echo") == 0)
        {
            EchoController::SendResponse(connectedClient,resp);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().empty())
        {
            char* newCharPointer = CreateEmptyResponse(Globals::successResponse);

            send(connectedClient.socket,newCharPointer,static_cast<int>(strlen(newCharPointer)),0);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET)
        {
            char* newCharPointer = CreateEmptyResponse(Globals::errorResponse);

            send(connectedClient.socket,newCharPointer,static_cast<int>(strlen(newCharPointer)),0);
        }
    }

#ifdef _WIN64
    closesocket(connectedClient.socket);
#else
    close(connectedClient.socket);
#endif
}


void SetupDirectory(char* directoryName)
{

}

// argv == parameter
// argc == amount of character executable call
int main(int argc, char **argv)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        // printf("%s\n",argv[i]);

        // Setup directory for using with FileController endpoint
        if(strcmp(argv[i],"--directory") == 0 && i + 1 < argc)
        {
            printf("Setup directory: %s\n", argv[i+1]);
            SetupDirectory(argv[i+1]);
        }
    }
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here!\n";


    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"Error at WSAStartup()\n");
        return 1;
    }
    SocketWrapper serverSocketWrapper;

    // Uncomment this block to pass the first stage
    serverSocketWrapper.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocketWrapper.socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors

    const char reuse = SO_KEEPALIVE;

    if (setsockopt(serverSocketWrapper.socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) \
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(serverSocketWrapper.socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port " << port << "\n" << std::endl;
        return 1;
    }

    int connection_backlog = 5;
    if (listen(serverSocketWrapper.socket, connection_backlog) != 0)
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
        clients.push_back(AcceptConnection(serverSocketWrapper.socket));

        HandleRequest(clients.back());

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
