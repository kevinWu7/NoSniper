cmake_minimum_required (VERSION 3.1)
project(NoSniper)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/util/*.cpp"
)

# 添加include目录
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/util")
include_directories("${CMAKE_SOURCE_DIR}/external/asio")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES} )

# 设置asio的宏定义
target_compile_definitions(${PROJECT_NAME} PRIVATE -DASIO_STANDALONE)

# 添加zlib的源文件（.c）到编译中
add_subdirectory(external/zlib)
# 添加mini的源文件（.c）到编译中
add_subdirectory(external/minizip)

target_link_libraries(${PROJECT_NAME} PRIVATE zlib)# 链接zlib库
target_link_libraries(${PROJECT_NAME} PRIVATE minizip)# 链接minizip库

#因为asio跨平台需要用到各个平台的网络库，所以要加上相关链接
if (CMAKE_SYSTEM_NAME STREQUAL "Linux") 
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()



 