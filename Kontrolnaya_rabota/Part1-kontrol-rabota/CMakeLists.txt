cmake_minimum_required(VERSION 3.0)

project(Part1-kontrol-rabota)

find_package(OpenCV REQUIRED)
include_directories(Part1 ${OpenCV_INCLUDE_DIRS})

add_executable(Part1-kontrol-rabota part1.cpp)
target_link_libraries(Part1-kontrol-rabota ${OpenCV_LIBS})