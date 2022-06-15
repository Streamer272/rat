cmake_minimum_required(VERSION 3.22)
project(rat C)

set(CMAKE_C_STANDARD 23)

add_executable(rat
        src/main.c
        src/def/options.h src/def/program.h src/def/style.h
        src/print_file.c src/print_file.h
        src/print_dir.c src/print_dir.h
        src/print_help.c src/print_help.h
        src/term.c src/term.h
        src/str_contains.c src/str_contains.h
        src/alloc.c src/alloc.h)

install(TARGETS rat DESTINATION /usr/local/bin)
