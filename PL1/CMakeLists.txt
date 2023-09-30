cmake_minimum_required(VERSION 3.25)
project(PL1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Exercise1 exercise1.c)
add_executable(Exercise3 exercise3.c)
add_executable(Exercise4 exercise4.c)

target_link_libraries(Exercise1 pthread)
target_link_libraries(Exercise3 pthread)
target_link_libraries(Exercise4 pthread)
