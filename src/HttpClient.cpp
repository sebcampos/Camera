//
// Created by Sebastian Campos on 3/31/24.
//

#include "../headers/HttpClient.h"


/**
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return
 */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * Parses a json string into key value pairs storing them in a hashmap.
 * @param rawJson json string from the create object detection event endpoint
 * @return hashmap containing the key value pairs of the event
 */
static std::map<std::string, std::string> parseJsonObject(std::string rawJson)
{
    std::map<std::string, std::string> res;
    size_t pos = 0;
    std::string token;
    std::string delimiter = ",";
    while ((pos = rawJson.find(delimiter)) != std::string::npos) {
        token = rawJson.substr(0, pos);
        std::regex rgx("\"(.+?)\".*?:[ \"]*([^\"]+)");
        std::smatch matches;
        if(std::regex_search(token, matches, rgx)) {
            res.emplace(matches[1], matches[2]);
        }
        rawJson.erase(0, pos + delimiter.length());
    }
    std::regex rgx("\"(.+?)\".*?:[ \"]*([^\"]+)\\}");
    std::smatch matches;
    if(std::regex_search(rawJson, matches, rgx)) {
        res.emplace(matches[1], matches[2]);
    }
    return res;

}

/**
 *
 * @param rawJson
 * @return
 */
static std::vector<std::string> parseJsonArray(const std::string& rawJson)
{
    std::regex rgx(R"("tracked".*?:.*?\[(.*?)\])");
    std::string resultString;
    std::string delimiter = ",";
    std::smatch matches;
    size_t pos = 0;
    std::string token;
    std::vector<std::string> result;
    if(std::regex_search(rawJson, matches, rgx)) {
        resultString = matches[1];
    }
    while ((pos = resultString.find(delimiter)) != std::string::npos)
    {
        token = resultString.substr(0, pos);
        std::regex rgx2(R"("(.+?)\")");
        std::smatch matches2;
        if(std::regex_search(token, matches2, rgx2)) {
            result.push_back(matches2[1]);
        }
        resultString.erase(0, pos + delimiter.length());
    }
    std::regex rgx2(R"("(.+?)\")");
    std::smatch matches2;
    if(std::regex_search(resultString, matches2, rgx2)) {
        result.push_back(matches2[1]);
    }
    return result;
}

std::vector<std::string> HttpClient::getTrackingList()
{

    CURLcode ret;
    CURL *hnd;
    struct curl_slist *slist1;
    std::string readBuffer;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, "http://cameraserver.local/api/cam/track");
    // curl_easy_setopt(hnd, CURLOPT_URL, "http://cameraserver.local/api/cam/events");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

    ret = curl_easy_perform(hnd);

    std::cout << ret << std::endl;
    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(slist1);
    slist1 = NULL;
    return  parseJsonArray(readBuffer);
}


std::string HttpClient::createObjectDetectionEvent(int objectIndex)
{
    // std::ostringstream payload;
    // payload << "{\"object_index\": " << objectIndex << "}";
    std::string payload = "{\"object_index\": " + std::to_string(objectIndex) + "}";
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *slist1;
    std::string readBuffer;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    //std::cout << payload.str() << std::endl;
    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, "http://cameraserver.local/api/cam/events/");
    // curl_easy_setopt(hnd, CURLOPT_URL, "http://cameraserver.local/api/cam/events");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, payload.c_str());
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
    std::map<std::string, std::string> res = parseJsonObject(readBuffer);
    std::cout << res["event_id"] << std::endl;
    return res["event_id"];
}

std::string HttpClient::updateObjectDetectionEvent(const std:: string& eventId, bool eventOccurring)
{
    std::string payload =  "{\"event_occuring\": " + std::to_string(eventOccurring) + "}";
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *slist1;
    std::string readBuffer;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    std::string url = "http://cameraserver.local/api/cam/events/" + eventId + "/";
    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
    // curl_easy_setopt(hnd, CURLOPT_URL, "http://cameraserver.local/api/cam/events");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

    ret = curl_easy_perform(hnd);

    std::cout << ret << std::endl;
    curl_easy_cleanup(hnd);
    hnd = nullptr;
    curl_slist_free_all(slist1);
    slist1 = nullptr;
    std::map<std::string, std::string> res = parseJsonObject(readBuffer);
    return res["event_id"];
}