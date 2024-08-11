//
// Created by esmayl on 11/08/2024.
//

#include "WebServer.h"

#include <utility>

WebServer::WebServer(int port, int receiveBufferSize)
{
    this->port = port;
    this->receiveBuffer = new char[receiveBufferSize];
    this->receiveBufferSize = receiveBufferSize;
}

HttpPacket WebServer::ParseRequestHeader(const std::string &rawString)
{
    return HttpPacket(rawString);
}

SocketWrapper WebServer::AcceptConnection(SocketWrapper connectedClient)
{
    struct sockaddr_in client_addr;

    int client_addr_len = sizeof(client_addr);

#ifdef _WIN64
    int casted_client_addr_len = sizeof(client_addr);
#else
    socklen_t casted_client_addr_len = client_addr_len;
#endif

    SocketWrapper returnSocket;
    returnSocket.socket = accept(connectedClient.socket, reinterpret_cast<sockaddr*>(&client_addr), &casted_client_addr_len);
    returnSocket.reuse = 1;

    return returnSocket;
}

void WebServer::HandleRequest(SocketWrapper connectedClient)
{

    ssize_t readBytes = 0;
    std::string s;

    if((readBytes = recv(connectedClient.socket,this->receiveBuffer,this->receiveBufferSize,0)) > 0)
    {
        receiveBuffer[readBytes] = '\0';
        s.append(receiveBuffer,readBytes);

        HttpPacket resp = ParseRequestHeader(s);

        // std::cout << "Received: " << resp.GetEndpoint() << std::endl;

        if(resp.GetRequestType() == HTTPMETHOD::GET && resp.GetEndpoint().compare(0,5,"files") == 0)
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

            send(connectedClient.socket,emptyResponse.c_str(),static_cast<int>(emptyResponse.length()),0);
        }
        else if(resp.GetRequestType() == HTTPMETHOD::GET)
        {
            std::string errorResponse = Globals::BuildResponse("",CONTENTTYPE::PLAIN, false);

            send(connectedClient.socket,errorResponse.c_str(),static_cast<int>(errorResponse.length()),0);
        }
    }

#ifdef _WIN64
    closesocket(connectedClient.socket);
#else
    close(connectedClient.socket);
#endif
}


void WebServer::SetupDirectory(std::string folderRoot)
{
    fileControllerInstance = new FileController(std::move(folderRoot));
}
