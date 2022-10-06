cmake_minimum_required(VERSION 3.22)
project(MySTL)

set(CMAKE_CXX_STANDARD 14)

include_directories(MySTL)

add_executable(MySTL
        MySTL/Alloc.h
        MySTL/Allocator.h
        MySTL/Detail/Alloc.cpp
        MySTL/main.cpp)
