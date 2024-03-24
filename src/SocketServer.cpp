//
// Created by Sebastian Campos on 3/15/24.
//

#include "../headers/SocketServer.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define MB 2591 * 1944



SocketServer::SocketServer()
{

    shutDown = false;
    camera = new Camera(0);

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

SocketServer::~SocketServer()
{
        delete camera;
}


int SocketServer::getAvailableConnection()
{
    for (int i = 0; i < 4; i++)
    {
        if (connections[i] == 0)
        {
            return i;
        }
    }
    return -1;
}


void handleResponse(int (&connections)[4], int streamIndex, char responseBuffer[1024])
{
    if (strcmp(responseBuffer, "close") == 0)
    {
        connections[streamIndex] = 0;
    }
    else if (strcmp(responseBuffer, "refresh") == 0)
    {
        // refresh current data
    }
}

void SocketServer::startStream(int streamIndex, int clientSocket)
{
    std::cout << "connecting to client at index `" << streamIndex << "`" << std::endl;
    connections[streamIndex] = clientSocket;
    char responseBuffer[1024] = {0};
    while (connections[streamIndex] == clientSocket)
    {
        std::vector<uchar> buffer;
        buffer.resize(MB);
        imencode(".png", camera->getCurrentFrame(), buffer);
        int buffer_size = buffer.size();
        std::string message = "buffer size and data " + std::to_string(buffer_size);
        char* char_array = new char[message.length() + 1];

        // copying the contents of the
        // string to char array
        strcpy(char_array, message.c_str());

        std::cout << "sending size to client at index `" << streamIndex << "`" << std::endl;
        send(clientSocket, char_array, std::strlen(char_array), 0);

        std::cout << "receiving response from  `" << streamIndex << "`" << std::endl;
        recv(clientSocket, responseBuffer, sizeof(responseBuffer), 0);
        handleResponse(connections, streamIndex, responseBuffer);

        std::cout << "sending image to client at index `" << streamIndex << "`" << std::endl;
        send(clientSocket, buffer.data(), buffer.size(), 0);

        std::cout << "receiving response from  `" << streamIndex << "`" << std::endl;
        recv(clientSocket, responseBuffer, sizeof(responseBuffer), 0);
        handleResponse(connections, streamIndex, responseBuffer);

        delete[] char_array;
    }
    std::cout << "disconnecting client at index `" << streamIndex << "`" << std::endl;


}




void SocketServer::run()
{
    std::cout << "starting camera thread" << std::endl;
    std::thread cameraThread = std::thread(&Camera::processFeed, camera);
    std::cout << "starting server" << std::endl;
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
            if (newConnectionIndex == -1)
            {
                std::cout << "Max connections reached" << std::endl;
                continue;
            }
            threads[newConnectionIndex] = new std::thread(&SocketServer::startStream, this, newConnectionIndex, clientSocket);

        }
        else if (strcmp(buffer, "shutDown") == 0)
        {
            // close all connections
            shutDown = true;
        }

    }

    for (int i = 0; i < 4; i++)
    {
        if (connections[i] == 0)
        {
            continue;
        }
        std::cout << "Closing thread: " << i << std::endl;
        connections[i] = -1;
        threads[i]->join();
    }

    // closing the socket.
    camera->stopFeed();
    cameraThread.join();
    close(serverSocket);
}

