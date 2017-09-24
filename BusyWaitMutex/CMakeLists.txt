cmake_minimum_required(VERSION 3.7)
project(BusyWaitMutex)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c mutex/lamports_bakery.h mutex/lamports_bakery.c list/sorted_list.h list/sorted_list.c utils.h utils.c stack/stack.h stack/stack.c)
add_executable(BusyWaitMutex ${SOURCE_FILES})

target_link_libraries(BusyWaitMutex pthread)
