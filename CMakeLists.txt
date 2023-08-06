cmake_minimum_required (VERSION 3.1)
project(NoSniper)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 添加可执行文件，将源文件和头文件分开
file(GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/include/external/zlib/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# 添加include目录
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/external/asio")
include_directories("${CMAKE_SOURCE_DIR}/include/external/zlib")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES} )

# 设置asio的宏定义
target_compile_definitions(${PROJECT_NAME} PRIVATE -DASIO_STANDALONE)

#因为asio跨平台需要用到各个平台的网络库，所以要加上相关链接
if (CMAKE_SYSTEM_NAME STREQUAL "Linux") 
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()



 