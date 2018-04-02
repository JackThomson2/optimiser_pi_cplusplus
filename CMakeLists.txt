cmake_minimum_required(VERSION 3.9)
project(pi_c__)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(pi_c__ main.cpp server.cpp server.h sensor.cpp sensor.h mpuManager.cpp mpuManager.h storageManager.cpp storageManager.h catch.hpp json.hpp)