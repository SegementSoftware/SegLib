cmake_minimum_required(VERSION 3.10)
project(SegLib)

set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} SegLib.cpp SegLibNumerical.cpp)