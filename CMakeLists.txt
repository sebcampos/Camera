cmake_minimum_required(VERSION 3.27)
project(webcamera)

find_package(OpenCV)
find_package(tensorflow-lite CONFIG)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

add_executable(webcamera main.cpp)
target_link_libraries(webcamera ${OpenCV_LIBS})
