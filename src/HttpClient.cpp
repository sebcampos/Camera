//
// Created by Sebastian Campos on 3/31/24.
//

#include "../headers/HttpClient.h"


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void HttpClient::createObjectDetectionEvent(int objectIndex)
{
    std::ostringstream payload;
    payload << "{\"object_index\": " << objectIndex << "}";
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *slist1;
    std::string readBuffer;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, "http://127.0.0.1:8000/api/cam/events/");
    // curl_easy_setopt(hnd, CURLOPT_URL, "http://cameraserver.local/api/cam/events");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, payload.str().c_str());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

    ret = curl_easy_perform(hnd);

    std::cout << ret << std::endl;
    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(slist1);
    slist1 = NULL;
}