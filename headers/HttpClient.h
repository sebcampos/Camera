//
// Created by Sebastian Campos on 3/31/24.
//

#ifndef WEBCAMERA_HTTPCLIENT_H
#define WEBCAMERA_HTTPCLIENT_H

#include <sstream>
#include <string>
#include <iostream>

#include <stdio.h>
#include <curl/curl.h>
#include <regex>
#include <map>

class HttpClient {
public:
    static std::string createObjectDetectionEvent(int objectIndex);
    static std::string updateObjectDetectionEvent(const std::string& eventId, bool eventOccurring);
    static std::vector<std::string> getTrackingList();

};


#endif //WEBCAMERA_HTTPCLIENT_H
