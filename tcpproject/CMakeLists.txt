cmake_minimum_required(VERSION 3.26)
project(tcpproject C)


SET(CMAKE_C_FLAGS -pthread)
add_compile_options(-pthread)
set(CMAKE_C_STANDARD 11)

add_executable(tcpproject main.c
        server.c)
