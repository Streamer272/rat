cmake_minimum_required(VERSION 3.22)
project(rat C)

set(CMAKE_C_STANDARD 23)

add_executable(rat
        src/main.c
        src/def/options.h
        src/alloc.c src/alloc.h src/def/program.h src/def/style.h
        src/print_file.c src/print_file.h
        src/print_help.c src/print_help.h
        src/term.c src/term.h)

install(TARGETS rat DESTINATION /usr/local/bin)
