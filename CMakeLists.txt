cmake_minimum_required(VERSION 3.8)
project(damo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -mavx -pthread")
#set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}") #默认静态编译

add_subdirectory(db)
add_subdirectory(pb)
aux_source_directory(. DIR_SRCS)

set(SOURCE_FILES main.cpp)
add_executable(damo ${SOURCE_FILES})

#link_directories("/usr/local/lib")  #添加编译链接的目录
target_link_libraries(damo db)
target_link_libraries(damo rocksdb)
target_link_libraries(damo protobuf)
target_link_libraries(damo pb)
target_link_libraries(damo grpc++)
target_link_libraries(damo grpc)
target_link_libraries(damo grpc++_reflection)
target_link_libraries(damo dl)