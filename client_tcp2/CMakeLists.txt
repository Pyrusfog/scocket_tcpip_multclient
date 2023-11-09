cmake_minimum_required(VERSION 3.26)
project(client_tcp2 C)


SET(CMAKE_C_FLAGS -pthread)
add_compile_options(-pthread)
set(CMAKE_C_STANDARD 11)

add_executable(client_tcp2 main.c)
