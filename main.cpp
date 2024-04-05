

#include<iostream>
#include "headers/SocketServer.h"


#include "headers/HttpClient.h"




int main()
{
    std::cout << "wut" << std::endl;
    HttpClient::createObjectDetectionEvent(0);
    return 0;
}


//int main() {
//    auto* server = new SocketServer();
//    server->run();
//    delete server;
//    return 0;
//}