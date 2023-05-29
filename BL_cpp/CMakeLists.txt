cmake_minimum_required(VERSION 3.10)
project(BL_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(BL_cpp main.cpp
                    source/BLproblem.cpp)

add_executable(Dynamic dynamic.cpp
        source/BLproblem.cpp)
add_executable(reverse_ofp reverse_problem/reverse_solver.cpp)

add_executable(add_noise reverse_problem/add_noise.cpp)


add_executable(reverse_noise reverse_problem/reverse_solver_noise.cpp)

add_executable(reverse_dynamic reverse_problem/reverse_dinamic.cpp)