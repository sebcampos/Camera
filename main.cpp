

#include<iostream>
#include "headers/SocketServer.h"


#include "headers/HttpClient.h"




//int main()
//{
//    std::cout << "wut" << std::endl;
//    //std::string newObjectId = HttpClient::createObjectDetectionEvent(1);
//    //std::string eventId = HttpClient::updateObjectDetectionEvent("93d5dadf-4c67-4c0c-a069-bf9e61201b8c", false);
//    //std::cout << eventId << std::endl;
//    std::vector<std::string> trackingList = HttpClient::getTrackingList();
//    return 0;
//}


int main() {
    auto* server = new SocketServer();
    server->run();
    delete server;
    return 0;
}