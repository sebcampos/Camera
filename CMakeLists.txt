cmake_minimum_required(VERSION 3.27)
project(webcamera)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})


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
        src/SocketServer.cpp)
#        local_test.cpp)
#        tests/test_modeldetection.cpp)
#        tests/test_2.cpp

target_link_libraries(webcamera ${OpenCV_LIBS})
target_link_libraries(webcamera tensorflowlite)