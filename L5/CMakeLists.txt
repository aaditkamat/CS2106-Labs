cmake_minimum_required(VERSION 3.12)
project(L5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(common)
include_directories(ex1)
include_directories(ex2)
include_directories(ex3)

add_executable(L5
        common/4files.img
        common/makefile
        common/reset.sh
        common/USFAT.h
        common/USFAT_Inspect.c
        common/USFAT_Util.c
        common/USFATI
        ex1/1000int.dat
        ex1/10int.dat
        ex1/ex1.c
        ex1/test1.in
        ex1/test1.out
        ex1/test2.in
        ex1/test2.out
        ex1/test3.in
        ex1/test3.out
        ex2/a.out
        ex2/alice.txt
        ex2/ex2.c
        ex2/fat.txt
        ex2/hello.c
        ex2/makefile
        ex2/mystery.abc
        ex2/rain.txt
        ex2/test.sh
        ex3/alice.txt
        ex3/ex3.c
        ex3/fat.txt
        ex3/hello.c
        ex3/makefile
        ex3/mystery.abc
        ex3/rain.txt
        ex3/test.sh)
