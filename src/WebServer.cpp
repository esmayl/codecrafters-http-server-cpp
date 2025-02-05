//
// Created by esmayl on 11/08/2024.
//

#include "WebServer.h"




// ReSharper disable once CppPossiblyUninitializedMember
WebServer::WebServer(int port): threadPool(5)
{
    this->port = port;
}

HttpPacket WebServer::ParseRequestHeader(const std::string &rawString)
{
    printf("Parsing request\n\n");

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

    // Using a single thread for accepting, probably not possible to make this multithreaded
    returnSocket.socket = accept(serverSocketWrapper.socket, reinterpret_cast<sockaddr*>(&client_addr), &casted_client_addr_len);
    returnSocket.reuse = 1;
    if (returnSocket.socket != INVALID_SOCKET)
    {
        // Create a new thread for every request
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
        printf("Error at WSAStartup()\n");
        return 1;
    }
#endif


    // Uncomment this block to pass the first stage
    serverSocketWrapper.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketWrapper.socket == 0)
    {
        printf("Failed to create server socket\n");
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors

    serverSocketWrapper.reuse = 1;

    if (setsockopt(serverSocketWrapper.socket, SOL_SOCKET, SO_REUSEADDR,&serverSocketWrapper.reuse , sizeof(serverSocketWrapper.reuse)) < 0)
    {
        printf("setsockopt failed\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(serverSocketWrapper.socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("Failed to bind to port ");
        printf(std::to_string(port).c_str());
        printf("\n");
        return 1;
    }

    int max_connection_backlog = 5;
    if (listen(serverSocketWrapper.socket, max_connection_backlog) != 0)
    {
        printf("listen failed\n");
        return 1;
    }

    // u_long mode =1;
    // ioctlsocket(ListenSocket,FIONBIO,&mode); // non - blocking

    printf("Waiting for a client to connect on port: ");
    printf(std::to_string(this->port).c_str());
    printf("\n");

    // ReSharper disable once CppDFAEndlessLoop
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
    printf("Handling request\n");

    char receiveBuffer[1024];

    size_t readBytes = 0;

    if((readBytes = recv(connectedClient->socket,receiveBuffer,sizeof(receiveBuffer) - 1,0)) > 0)
    {
        receiveBuffer[readBytes] = '\0';
        std::string s(receiveBuffer, readBytes);;

        HttpPacket requestPacket = ParseRequestHeader(s);

        printf("Endpoint hit: ");
        printf(requestPacket.GetEndpoint().c_str());
        printf("\n");
        std::string endpoint = requestPacket.GetEndpoint();

        //TODO: Create controller that communicated with sql server
        //TODO: Add performance metric in all responses, amount of seconds past since receiving request until sending response
        //TODO: Make video stream endpoint, mount folder from router and stream out maybe hls? multicast?
        if(canUseFiles && endpoint.compare(0,7,"/files/") == 0)
        {
            if(requestPacket.GetRequestType() == HTTPMETHOD::GET)
            {
                this->fileControllerInstance->GetResponse(&requestPacket,connectedClient,endpoint.substr(6).c_str());
            }
            else if(requestPacket.GetRequestType() == HTTPMETHOD::POST)
            {
                this->fileControllerInstance->PostResponse(&requestPacket,connectedClient,endpoint.substr(6).c_str(), requestPacket.GetBody(), requestPacket.GetBodyLength());
            }
        }
        else if(requestPacket.GetRequestType() == HTTPMETHOD::GET && endpoint.compare(0,11,"/user-agent") == 0)
        {
            UserAgentController::SendResponse(connectedClient,&requestPacket);
        }
        else if(requestPacket.GetRequestType() == HTTPMETHOD::GET && endpoint.compare(0,5,"/echo") == 0)
        {
            EchoController::SendResponse(connectedClient,&requestPacket);
        }
        else if(requestPacket.GetRequestType() == HTTPMETHOD::GET && endpoint.empty())
        {
            std::string emptyResponse = Globals::BuildResponse(&requestPacket,Globals::getSuccessResponse,"", CONTENTTYPE::PLAIN, true);

            send(connectedClient->socket,emptyResponse.c_str(),static_cast<int>(emptyResponse.length()),0);
        }
        else if(requestPacket.GetRequestType() == HTTPMETHOD::GET)
        {
            std::string errorResponse = Globals::BuildResponse(&requestPacket,Globals::errorResponse,"", CONTENTTYPE::PLAIN, false);

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
