cmake_minimum_required(VERSION 3.26)
project(servertcp C)

set(CMAKE_C_STANDARD 11)
SET(CMAKE_C_FLAGS -pthread)
add_compile_options(-pthread)
#set(THREADS_PREFER_PTHREAD_FLAG ON)
add_executable(servertcp main.c)
