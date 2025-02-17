
cmake_minimum_required(VERSION 3.10)
project(WebServer)


include_directories(
    base
    tcp
)

SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

find_package(Threads REQUIRED)

aux_source_directory(base SRC_LIST1)
aux_source_directory(tcp SRC_LIST2)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/test/)
file(GLOB ECHO_SERVER_SOURCES example/echo_server/*.cpp)

add_executable(echoserver ${ECHO_SERVER_SOURCES} ${SRC_LIST1} ${SRC_LIST2} )
target_link_libraries(echoserver ${CMAKE_THREAD_LIBS_INIT})