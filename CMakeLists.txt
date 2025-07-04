cmake_minimum_required(VERSION 3.12)

project(radixsort)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

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
find_package(Threads)

set(BENCH_SRCS
	bench/main.cpp
	thirdparty/nonius/nonius.hpp
	)

add_executable(bench ${CSRCS} ${CPPSRCS} ${BENCH_SRCS})
target_link_libraries(bench PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(bench PRIVATE thirdparty/nonius)

set(TEST_SRCS
	test/test_radixsortcpp.cpp
	test/main.cpp
	)

if(NOT MSVC)
	list( APPEND TEST_SRCS
		test/test_radixsortc.cpp
		)
endif()

add_executable(tests ${CSRCS} ${CPPSRCS} ${TEST_SRCS})
target_include_directories(tests PRIVATE thirdparty/Catch)
set_target_properties(tests PROPERTIES OUTPUT_NAME test)
