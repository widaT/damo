# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
add_library (db ${DIR_LIB_SRCS})
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")       # 用IF控制，可以实现Release版本不输出debug信息
    add_definitions(-DDEBUG)
endif()