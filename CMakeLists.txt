# matrix_lib/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(matrix C)

add_library(matrix
    src/matrix.c
)

target_include_directories(matrix
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(matrix
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wpedantic
)
