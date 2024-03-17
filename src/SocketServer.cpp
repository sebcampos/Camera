//
// Created by Sebastian Campos on 3/15/24.
//

#include "../headers/SocketServer.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>



SocketServer::SocketServer(Camera* videoCam)
{

    shutDown = false;
    camera = videoCam;

    // creating socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    int bindNum = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    std::cout << "Bind num: " << bindNum << std::endl;
    // listening to the assigned socket
    listen(serverSocket, 5);

}


int SocketServer::getAvailableConnection()
{
    for (int i = 0; i < 4; i++)
    {
        if (connections[i] == -1)
        {
            return i;
        }
    }
    return -1;
}


void SocketServer::startStream(int streamIndex, int clientSocket)
{
    bool keepConnected = true;
    if (streamIndex == -1)
    {
        std::cout << "max connections reached" << std::endl;
        return;
    }
    connections[streamIndex] = clientSocket;
    char cameraBuffer[1024] = "hello client";
    char responseBuffer[1024] = {0};
    while (keepConnected)
    {
        send(clientSocket, cameraBuffer, sizeof(cameraBuffer), 0);
        recv(clientSocket, responseBuffer, sizeof(responseBuffer), 0);
        if (strcmp(responseBuffer, "close") == 0)
        {
            keepConnected = false;
        }
        else if (strcmp(responseBuffer, "refresh") == 0)
        {
            // refresh current data
        }
    }

    connections[streamIndex] = clientSocket;

}

void SocketServer::run()
{

    while (!shutDown) {
        // accepting connection request
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        int newConnectionIndex;
        // receiving data
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        if (strcmp(buffer, "connect") == 0)
        {
            // set up connection
            newConnectionIndex = getAvailableConnection();
            startStream(newConnectionIndex, clientSocket);

        }
        else if (strcmp(buffer, "shutDown") == 0)
        {
            // close all connections
            shutDown = true;
        }

    }
    // closing the socket.
    close(serverSocket);
}