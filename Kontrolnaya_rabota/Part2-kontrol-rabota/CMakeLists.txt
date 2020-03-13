cmake_minimum_required(VERSION 3.0)

project(Part2-kontrol-rabota)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Part2-kontrol-rabota part2.cpp)
target_link_libraries(Part2-kontrol-rabota ${OpenCV_LIBS})