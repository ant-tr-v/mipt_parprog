cmake_minimum_required(VERSION 3.10)
project(omp_matrix)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")


add_executable(mat_mul mat_mul.cpp Matrix.cpp Vector.cpp Vector.hpp)