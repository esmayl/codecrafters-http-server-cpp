//
// Created by esmayl on 11/08/2024.
//

#include "WebServer.h"




WebServer::WebServer(int port): threadPool(30)
{
    this->port = port;
}

HttpPacket WebServer::ParseRequestHeader(const std::string &rawString)
{
    std::cout << "Parsing request" << std::endl;

    return HttpPacket(rawString);
}

void WebServer::AcceptConnection()
{
    struct sockaddr_in client_addr;

    int client_addr_len = sizeof(client_addr);

#ifdef _WIN64
    int casted_client_addr_len = sizeof(client_addr);
#else
    socklen_t casted_client_addr_len = client_addr_len;
#endif

    SocketWrapper returnSocket;
    returnSocket.socket = accept(serverSocketWrapper.socket, reinterpret_cast<sockaddr*>(&client_addr), &casted_client_addr_len);
    returnSocket.reuse = 1;
    if (returnSocket.socket >= 0)
    {

        threadPool.Enqueue([this,returnSocket]() mutable
        {
            HandleRequest(&returnSocket);
        });
    }
    else
    {
        std::cerr << "Failed to accept connection" << std::endl;
    }
}

int WebServer::Start()
{

#ifdef _WIN64
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"Error at WSAStartup()\n");
        return 1;
    }
#endif


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
    server_addr.sin_port = htons(port);

    if (bind(serverSocketWrapper.socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port " << port << "\n" << std::endl;
        return 1;
    }

    int max_connection_backlog = 20;
    if (listen(serverSocketWrapper.socket, max_connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
    }

    // u_long mode =1;
    // ioctlsocket(ListenSocket,FIONBIO,&mode); // non - blocking

    std::cout << "Waiting for a client to connect...\n";

    while(true)
    {
        AcceptConnection();
    }

#ifdef _WIN64
    closesocket(serverSocketWrapper.socket);

    WSACleanup();
#else
    close(serverSocketWrapper.socket);
#endif

    return 0;
}

void WebServer::HandleRequest(SocketWrapper* connectedClient)
{
    std::cout << "Handling request\n";

    char receiveBuffer[1024];

    ssize_t readBytes = 0;
    std::string s;

    if((readBytes = recv(connectedClient->socket,receiveBuffer,sizeof(receiveBuffer) - 1,0)) > 0)
    {
        std::cout << "Custom Received1: " << readBytes << std::endl;

        receiveBuffer[readBytes] = '\0';
        s.append(receiveBuffer,readBytes);

        HttpPacket resp = ParseRequestHeader(s);

        std::cout << "Custom Received: " << resp.GetEndpoint() << std::endl;

        if(canUseFiles && resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().compare(0,5,"files") == 0)
        {
            this->fileControllerInstance->SendResponse(connectedClient,resp.GetEndpoint().substr(5+1).c_str());
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().compare(0,10,"user-agent") == 0)
        {
            UserAgentController::SendResponse(connectedClient,resp);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().compare(0,4,"echo") == 0)
        {
            EchoController::SendResponse(connectedClient,resp);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().empty())
        {
            std::string emptyResponse = Globals::BuildResponse("",CONTENTTYPE::PLAIN, true);

            std::cout << "Custom Sending: " << emptyResponse.c_str() << std::endl;

            send(connectedClient->socket,emptyResponse.c_str(),static_cast<int>(emptyResponse.length()),0);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET)
        {
            std::string errorResponse = Globals::BuildResponse("",CONTENTTYPE::PLAIN, false);

            std::cout << "Custom Error Sending: " << errorResponse.c_str() << std::endl;

            send(connectedClient->socket,errorResponse.c_str(),static_cast<int>(errorResponse.length()),0);
        }
    }

#ifdef _WIN64
    closesocket(connectedClient->socket);
#else
    close(connectedClient->socket);
#endif
}


void WebServer::SetupDirectory(std::string folderRoot)
{
    fileControllerInstance = std::make_unique<FileController>(std::move(folderRoot));

    canUseFiles = true;
}
