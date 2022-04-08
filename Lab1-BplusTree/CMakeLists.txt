cmake_minimum_required(VERSION 3.21)
project(B___2_0)

set(CMAKE_CXX_STANDARD 20)

add_executable(B___2_0 main.cpp BplusTree.cpp BplusTree.h)

target_compile_options(B___2_0 PRIVATE -Wall -Wextra -Werror
        -Wno-missing-field-initializers
        -Wno-attributes
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-error=effc++
        )


