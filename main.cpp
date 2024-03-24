

#include<iostream>
#include "headers/SocketServer.h"


int main() {
    auto* server = new SocketServer();
    server->run();
    delete server;
    return 0;
}