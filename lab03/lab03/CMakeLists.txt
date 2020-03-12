cmake_minimum_required(VERSION 3.0)

project(lab03)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab03 lab03.cpp)
target_link_libraries(lab03 ${OpenCV_LIBS})