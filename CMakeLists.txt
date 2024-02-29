cmake_minimum_required(VERSION 3.27)
project(webcamera)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/Users/sebash/builds/tflite-dist/include")


# lib has 1 file: libtensorflowlite.dylib
ADD_LIBRARY (tensorflowlite SHARED IMPORTED)
set_property (TARGET tensorflowlite PROPERTY IMPORTED_LOCATION /Users/sebash/builds/tflite-dist/libs/mac/libtensorflowlite.dylib)

set(CMAKE_CXX_STANDARD 17)


add_executable(webcamera main.cpp
        src/Camera.cpp
        src/ObjectDetectionModel.cpp)
#
#add_executable(webcamera main.cpp
#        Camera.h
#        ObjectDetectionModel.h
#        DatabaseConnection.h
#        Camera.cpp
#        ObjectDetectionModel.cpp
#        DatabaseConnection.cpp
#        )
target_link_libraries(webcamera ${OpenCV_LIBS})
target_link_libraries (webcamera tensorflowlite)