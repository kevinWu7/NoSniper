cmake_minimum_required(VERSION 3.1)
project(zlib_thirdlib)

# 添加zlib的源文件（.c）到编译中
file(GLOB ZLIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c") 

# 创建一个静态库
add_library(${PROJECT_NAME}  STATIC ${ZLIB_SOURCES})

# 设置zlib的头文件路径，主程序zlib的头文件时能都自动检测该路径
target_include_directories(${PROJECT_NAME}  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")