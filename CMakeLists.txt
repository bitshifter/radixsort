cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

project(radixsort)

add_subdirectory(c)
add_subdirectory(cpp)
add_subdirectory(rust)
