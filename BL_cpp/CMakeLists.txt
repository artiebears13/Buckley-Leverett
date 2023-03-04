cmake_minimum_required(VERSION 3.24)
project(BL_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(BL_cpp main.cpp
                    source/BLproblem.cpp)
