cmake_minimum_required(VERSION 3.0)

project(lab02)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab02 Source.cpp)
target_link_libraries(lab02 ${OpenCV_LIBS})