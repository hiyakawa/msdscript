cmake_minimum_required(VERSION 3.24)
project(msdscript)

set(CMAKE_CXX_STANDARD 17)

add_executable(msdscript
        main.cpp tests.cpp
        cmdline.h pointer.h
        expr.h expr.cpp
        parse.h parse.cpp
        exec.h exec.cpp
        val.h val.cpp
        env.h env.cpp)
