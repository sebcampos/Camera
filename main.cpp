

#include<iostream>
#include "headers/SocketServer.h"




//int main()
//{
//
//    std::string newObjectId = HttpClient::createObjectDetectionEvent(1);
//    //std::vector<std::string> trackingList = HttpClient::getTrackingList();
//    return 0;
//}


int main() {
    auto* server = new SocketServer();
    server->run();
    delete server;
    return 0;
}