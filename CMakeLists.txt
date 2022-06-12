cmake_minimum_required(VERSION 3.22)
project(rat C)

set(CMAKE_C_STANDARD 23)

add_executable(rat
        src/main.c
        src/options.h src/bool.h)
