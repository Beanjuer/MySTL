cmake_minimum_required(VERSION 3.22)
project(MySTL)

set(CMAKE_CXX_STANDARD 14)

include_directories(MySTL)

add_executable(MySTL
        MySTL/main.cpp)
