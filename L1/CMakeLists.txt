cmake_minimum_required(VERSION 3.12)
project(L1)

set(CMAKE_CXX_STANDARD 14)

add_executable(L1
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/L1.cbp
        cmake-build-debug/Makefile
        ex1/ex1.c
        ex1/test1.in
        ex1/test1.out
        ex1/test2.in
        ex1/test2.out
        ex2/ex2.c
        ex2/test1.in
        ex2/test1.out
        ex2/test2.in
        ex2/test2.out
        ex3/ex3.c
        ex3/test1.in
        ex3/test1.out
        Lab1.pdf)
