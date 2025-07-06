cmake_minimum_required(VERSION 3.12)

project(radixsort)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

add_subdirectory(thirdparty/Catch2)

include_directories(src/cpp)

if(NOT MSVC)
	include_directories(src/c)
	set(CSRCS
		src/c/radixsort.c
		src/c/radixsort.h
		)
	add_definitions(-DHAVE_C99_SUPPORT)
endif()

set(CPPSRCS
	src/cpp/radixsort.hpp
	src/cpp/radixsort.inl
	)

# nonius requires threads
#find_package(Threads)

set(BENCH_SRCS
	bench/main.cpp
	#thirdparty/nonius/nonius.hpp
	)

add_executable(bench ${CSRCS} ${CPPSRCS} ${BENCH_SRCS})
target_link_libraries(bench PRIVATE Catch2::Catch2WithMain)

set(TEST_SRCS
	test/test_radixsortcpp.cpp
	)

if(NOT MSVC)
	list( APPEND TEST_SRCS
		test/test_radixsortc.cpp
		)
endif()

add_executable(tests ${CSRCS} ${CPPSRCS} ${TEST_SRCS})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
