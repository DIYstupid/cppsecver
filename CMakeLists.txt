cmake_minimum_required(VERSION 3.10)
project(cppsecver)

set(CMAKE_CXX_STANDARD 11)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/src)

# 查找 src 目录下所有 cpp 文件
file(GLOB SRC_FILES src/*.cpp)

add_executable(server server.cpp ${SRC_FILES})
add_executable(client client.cpp ${SRC_FILES})
add_executable(ThreadPooltest ThreadPooltest.cpp ${SRC_FILES})
add_executable(test test.cpp ${SRC_FILES})
# 链接 pthread 库（如有需要）
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)
