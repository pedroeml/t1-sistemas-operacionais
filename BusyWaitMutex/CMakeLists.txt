cmake_minimum_required(VERSION 3.7)
project(BusyWaitMutex)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c mutex.h mutex.c)
add_executable(BusyWaitMutex ${SOURCE_FILES})

target_link_libraries(BusyWaitMutex pthread)
