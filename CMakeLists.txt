cmake_minimum_required(VERSION 3.14)
project(hashmap)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "-std=c++11 -O3 -march=native")

#add_executable(hashmap main.cpp hashmap.h calbuffer.cpp)
add_executable(hashmap main.cpp hashmap.h)