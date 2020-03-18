cmake_minimum_required(VERSION 3.5)
project(matrix)

#include_directories(include)
#target_include_directories(matrix PRIVATE ${YOUR_DIRECTORY})
include_directories(.)

add_executable(matrix test/main.cpp matrix.hpp vector.hpp)
#find_package(OpenMP)
#if (OPENMP_FOUND)
#   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#   set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif ()
