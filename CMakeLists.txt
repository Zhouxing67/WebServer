
cmake_minimum_required(VERSION 3.10)
project(WebServer)

include_directories(
    base
    tcp
    logger
)

set(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS "-D_DEBUG -D_ANSYC_LOG  -Wall -Werror -fno-omit-frame-pointer -std=c++17 ")
# 设置 Debug 模式下的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-O1 ")
# 设置 Release 模式下的编译选项
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_BUILD_TYPE RELEASE)


find_package(Threads REQUIRED)

aux_source_directory(base SRC_LIST1)
aux_source_directory(tcp SRC_LIST2)
aux_source_directory(logger SRC_LIST3)
aux_source_directory(example/echo_server/ EXAMPLE_ECHOSERVER_LIST)


set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_executable(echoserver ${EXAMPLE_ECHOSERVER_LIST} ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3})
target_link_libraries(echoserver ${CMAKE_THREAD_LIBS_INIT})

add_executable(LogstreamTest ${SRC_LIST1} ${SRC_LIST3} "test/Log_test/Logstream_test.cpp")
target_link_libraries(echoserver ${CMAKE_THREAD_LIBS_INIT})