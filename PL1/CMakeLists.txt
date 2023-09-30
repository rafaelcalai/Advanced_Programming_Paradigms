cmake_minimum_required(VERSION 3.25)
project(PL1 C)

set(CMAKE_C_STANDARD 11)

add_executable(PL1 exercise3.c)

target_link_libraries(PL1 pthread)