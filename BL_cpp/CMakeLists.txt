cmake_minimum_required(VERSION 3.10)
project(BL_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(BL_cpp main.cpp
                    source/BLproblem.cpp)

add_executable(reverse_ofp reverse_problem/reverse_solver.cpp)
