cmake_minimum_required(VERSION 3.8)
project(mpi_lab)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_executable(a1_base 1a_baseline.cpp)
add_executable(a1_mpi 1a_mpi.cpp)
add_executable(a2_base 2a_baseline.cpp)
add_executable(a2_mpi 2a_mpi.cpp)