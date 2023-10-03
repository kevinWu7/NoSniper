cmake_minimum_required(VERSION 3.1)
project(minizip)

# 添加zlib的源文件（.c）到编译中
file(GLOB MINIZLIB_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/unzip.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ioapi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/miniunz.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/zip.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/minizip.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/mztools.c"
) 

# 创建一个静态库
add_library(${PROJECT_NAME}  STATIC ${MINIZLIB_SOURCES})

include_directories("${CMAKE_SOURCE_DIR}/external/zlib")

# 设置minizip的头文件路径，主程序minizip的头文件时能都自动检测该路径
target_include_directories(${PROJECT_NAME}  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

