cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

project(radixsort)

# Nonius requires Boost
find_package(Boost)
if(Boost_FOUND)
	find_package(Threads REQUIRED)
	add_subdirectory(bench)
endif()

add_subdirectory(test)
