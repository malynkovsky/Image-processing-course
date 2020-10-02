cmake_minimum_required(VERSION 3.0)

project(lab01)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab01 source.cpp)
target_link_libraries(lab01 ${OpenCV_LIBS})