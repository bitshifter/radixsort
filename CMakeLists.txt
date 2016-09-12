cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

project(radixsort)

# Nonius beta not building on MSVC right now
if(NOT MSVC)
	# Nonius requires Boost
	find_package(Threads)
	find_package(Boost)
	if(Boost_FOUND AND Threads_FOUND)
		add_subdirectory(bench)
	endif()
endif()

add_subdirectory(test)
