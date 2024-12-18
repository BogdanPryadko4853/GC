cmake_minimum_required(VERSION 3.30)
project(gc C)

set(CMAKE_C_STANDARD 11)

add_executable(gc main.c
        vm.h
        vm.c)
