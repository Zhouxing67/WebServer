
cmake_minimum_required(VERSION 3.10)
project(WebServer)

include_directories(
    src/base
    src/tcp
    src/logger
    src/timer
    src/eventloop
)

set(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS "-D_DEBUG -D_ANSYC_LOG  -Wall -Werror -fno-omit-frame-pointer -std=c++17 ")
# 启用 Clang 的颜色诊断
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
# 指定使用 LLD 作为链接器
set(CMAKE_LINKER "lld")


# 设置 Debug 模式下的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-O1 ")
# 设置 Release 模式下的编译选项
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_BUILD_TYPE RELEASE)


find_package(Threads REQUIRED)

aux_source_directory(src/base SRC_BASE)
aux_source_directory(src/tcp SRC_TCP)
aux_source_directory(src/logger SRC_LOGGER)
aux_source_directory(src/timer SRC_TIMER)
aux_source_directory(src/eventloop SRC_EVENTLOOP)


aux_source_directory(example/echo_server/ EXAMPLE_ECHOSERVER_LIST)


set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_executable(echoserver ${EXAMPLE_ECHOSERVER_LIST} ${SRC_BASE} ${SRC_LOGGER} ${SRC_TIMER} ${SRC_EVENTLOOP} ${SRC_TCP})
target_link_libraries(echoserver ${CMAKE_THREAD_LIBS_INIT})

add_executable(LogstreamTest ${SRC_BASE} ${SRC_TIMER} ${SRC_LOGGER} ${SRC_EVENTLOOP}  "test/Log_test/Logstream_test.cpp")
target_link_libraries(echoserver ${CMAKE_THREAD_LIBS_INIT})