cmake_minimum_required(VERSION 3.9)
project(pi_c__)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread -lbluetooth")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(usr/local/include)
link_directories(usr/local/lib)

add_executable(pi_c__ main.cpp server.cpp server.h sensor.cpp sensor.h 
	mpuManager.cpp mpuManager.h storageManager.cpp storageManager.h 
	catch.hpp json.hpp MPU6050.cpp MPU6050.h pgmspace.h I2Cdev.cpp
	I2Cdev.h multiplexer.cpp multiplexer.h )

target_link_libraries(pi_c__ bcm2835)