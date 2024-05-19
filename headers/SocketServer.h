//
// Created by Sebastian Campos on 3/15/24.
//

#ifndef WEBCAMERA_SOCKETSERVER_H
#define WEBCAMERA_SOCKETSERVER_H

#include "StreamCamera.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

class SocketServer {
public:
    bool shutDown;
    int serverSocket;
    int connections[4] = {-1, -1, -1, -1};
    bool handleResponse(int streamIndex, char* responseBuffer);
    std::thread* threads[4];
    StreamCamera* camera;
    sockaddr_in serverAddress{};
    SocketServer();
    ~SocketServer();
    void run();
private:
    int getAvailableConnection();
    void startStream(int streamIndex, int clientSocket);


};


#endif //WEBCAMERA_SOCKETSERVER_H
