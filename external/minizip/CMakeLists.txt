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

# 添加条件编译，仅在Windows平台编译这两个文件
if(WIN32)
    file(GLOB WINDOWS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/iowin32.c")
    target_sources(${PROJECT_NAME} PRIVATE ${WINDOWS_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE MINIZIP_USEWIN32) #MINIZIP_USEWIN32自定义的，可在代码中拿来判断
endif()
