cmake_minimum_required(VERSION 3.22)

project(Sparse_matrices_and_numbers)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Werror")

enable_testing()

add_executable(main main.cpp)

add_subdirectory(Rational_number)

add_subdirectory(Matrix)

add_subdirectory(Exceptions)

target_link_libraries(main Rational_number Matrix Exceptions)