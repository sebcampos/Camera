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

class HttpClient {
public:
    static void createObjectDetectionEvent(int objectIndex);
    static void updateObjectDetectionEvent();

};


#endif //WEBCAMERA_HTTPCLIENT_H
