cmake_minimum_required(VERSION 3.20)
project(matrixmultiply)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/local/include/eigen3/Eigen)

add_executable(matrixmultiply main.cpp mult.h mult.cpp)
