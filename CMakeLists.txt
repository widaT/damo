cmake_minimum_required(VERSION 3.8)
project(damo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -mavx")

add_subdirectory(db)
add_subdirectory(pb)
aux_source_directory(. DIR_SRCS)

set(SOURCE_FILES main.cpp)
add_executable(damo ${SOURCE_FILES})

target_link_libraries(damo db rocksdb protobuf pb grpc++ grpc grpc++_reflection dl)