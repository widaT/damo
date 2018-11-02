cmake_minimum_required(VERSION 3.8)
project(damo)


add_subdirectory(db)
add_subdirectory(pb)
aux_source_directory(.. DIR_SRCS)

set(SOURCE_FILES main.cpp)

add_executable(damo ${SOURCE_FILES})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")       # 用IF控制，可以实现Release版本不输出debug信息
    add_definitions(-DDEBUG)
endif()

#link_directories("/usr/local/lib")  #添加编译链接的目录
target_link_libraries(damo db)
target_link_libraries(damo rocksdb)
target_link_libraries(damo protobuf)
target_link_libraries(damo pb)
target_link_libraries(damo grpc++)
target_link_libraries(damo grpc)
target_link_libraries(damo grpc++_reflection)
target_link_libraries(damo dl)