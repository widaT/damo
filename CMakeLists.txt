cmake_minimum_required(VERSION 3.8)
project(damo)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -mavx -pthread")
#set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}") #默认静态编译

add_subdirectory(src)