cmake_minimum_required(VERSION 3.27)
project(webcamera)
SET(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV)
find_package(CURL REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIR})

if(UNIX AND NOT APPLE)
        include_directories("/home/sebash/tflite-dist/include")
        ADD_LIBRARY (tensorflowlite SHARED IMPORTED)
        set_property (TARGET tensorflowlite PROPERTY IMPORTED_LOCATION /home/sebash/tflite-dist/libs/linux/libtensorflowlite.so)
endif()

if (UNIX AND APPLE)
        include_directories("/Users/sebash/builds/tflite-dist/include")
        ADD_LIBRARY (tensorflowlite SHARED IMPORTED)
        set_property (TARGET tensorflowlite PROPERTY IMPORTED_LOCATION /Users/sebash/builds/tflite-dist/libs/mac/libtensorflowlite.dylib)
endif()

set(CMAKE_CXX_STANDARD 17)

add_executable(webcamera
        main.cpp
        src/Camera.cpp
        src/ObjectDetectionModel.cpp
        src/SocketServer.cpp
        src/HttpClient.cpp)
#        tests/test_http_client.cpp)
#        local_test.cpp)
#        tests/test_modeldetection.cpp)
#        tests/test_2.cpp

target_link_libraries(webcamera ${OpenCV_LIBS})
target_link_libraries(webcamera tensorflowlite)
target_link_libraries(webcamera CURL::libcurl)