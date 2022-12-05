cmake_minimum_required(VERSION 3.23)
project(untitled C)

set(CMAKE_C_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(lab9.0 openmp_petle_simple.c)

target_link_libraries(lab9.0 PRIVATE OpenMP::OpenMP_C)
