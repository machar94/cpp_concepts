cmake_minimum_required(VERSION 3.5)
project(cpp_concepts VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/templates)
add_subdirectory(src/threading)