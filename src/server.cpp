#include <iostream>
#include <cstdlib>
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
#include <stdio.h>

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


#ifdef _WIN64
    SOCKET AcceptConnection(SOCKET serverSocket)
    {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        return accept(serverSocket, (struct sockaddr *) &client_addr,(socklen_t *)&client_addr_len );
    }

    void HandleRequest(SOCKET connectedClient)
    {
        char receiveBuffer[4096];

        ssize_t readBytes = 0;
        std::string s;

        if((readBytes = recv(connectedClient,receiveBuffer,sizeof receiveBuffer - 1 ,0)) > 0)
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

                send(connectedClient,newCharPointer,strlen(newCharPointer),0);
            }
            else if(resp.GetRequestType() == HTTPMETHOD::GET)
            {
                char* newCharPointer = CreateEmptyResponse(Globals::errorResponse);

                send(connectedClient,newCharPointer,strlen(newCharPointer),0);
            }
        }
        closesocket(connectedClient);
    }

#else
    int AcceptConnection(int server_fd)
    {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        return accept(server_fd, (struct sockaddr *) &client_addr,(socklen_t *)&client_addr_len );
    }

    void HandleRequest(int& connectedClient)
    {
        char receiveBuffer[4096];

        ssize_t readBytes = 0;
        std::string s;

        if((readBytes = recv(connectedClient,receiveBuffer,sizeof receiveBuffer - 1 ,0)) > 0)
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

                send(connectedClient,newCharPointer,strlen(newCharPointer),0);
            }
            else if(resp.GetRequestType() == HTTPMETHOD::GET)
            {
                char* newCharPointer = CreateEmptyResponse(Globals::errorResponse);

                send(connectedClient,newCharPointer,strlen(newCharPointer),0);
            }
        }

        close(connectedClient);
    }
#endif


int main(int argc, char **argv)
{

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

    // Uncomment this block to pass the first stage
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors

    const char reuse = SO_KEEPALIVE;

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) \
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

#else

    // Uncomment this block to pass the first stage
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors

    int reuse = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

#endif

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

#ifdef _WIN64
    if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }

    int connection_backlog = 5;
    if (listen(serverSocket, connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
    }

    // u_long mode =1;
    // ioctlsocket(ListenSocket,FIONBIO,&mode); // non - blocking

    std::cout << "Waiting for a client to connect...\n";

    std::vector<SOCKET> clients;

    while(true)
    {
        clients.push_back(AcceptConnection(serverSocket));

        HandleRequest(clients.back());

        clients.pop_back();
    }

    close(serverSocket);

    WSACleanup();

#else

  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0)
  {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0)
  {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  
  std::cout << "Waiting for a client to connect...\n";
  std::vector<int> clients;

  while(true)
  {
      clients.push_back(AcceptConnection(server_fd));
      HandleRequest(clients.back());
      clients.pop_back();
  }

  close(server_fd);

#endif

  return 0;
}