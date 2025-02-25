
cmake_minimum_required(VERSION 3.10)
project(WebServer)

include_directories(
    base
    tcp
)

set(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

find_package(Threads REQUIRED)

aux_source_directory(base SRC_LIST1)
aux_source_directory(tcp SRC_LIST2)
aux_source_directory(logger SRC_LIST3)
aux_source_directory(example/echo_server/ EXAMPLE_ECHOSERVER_LIST)


set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/test/)

add_executable(echoserver ${EXAMPLE_ECHOSERVER_LIST} ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3})
target_link_libraries(echoserver ${CMAKE_THREAD_LIBS_INIT})