cmake_minimum_required(VERSION 3.0)

project(lab04)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab04 source.cpp)
target_link_libraries(lab04 ${OpenCV_LIBS})