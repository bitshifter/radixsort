cmake_minimum_required(VERSION 2.8.12)

if (NOT MSVC)
	# MSVC doesn't support C99 variable length arrays
	add_subdirectory(c)
endif()
add_subdirectory(cpp)
