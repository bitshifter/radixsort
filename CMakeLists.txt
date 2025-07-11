cmake_minimum_required(VERSION 3.12)

project(radixsort)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

include(CTest)

add_subdirectory(thirdparty/Catch2)

include_directories(src/cpp)

include_directories(src/c)
set(CSRCS
	src/c/radixsort.c
	src/c/radixsort.h
	)
add_definitions(-DHAVE_C99_SUPPORT)

set(CPPSRCS
	src/cpp/radixsort.hpp
	src/cpp/radixsort.inl
	)

set(BENCH_SRCS
	bench/bench_radixsort.cpp
	)

add_executable(bench ${CSRCS} ${CPPSRCS} ${BENCH_SRCS})
target_link_libraries(bench PRIVATE Catch2::Catch2WithMain)

set(TEST_SRCS
	test/test_radixsortcpp.cpp
	test/test_radixsortc.cpp
	)

add_executable(tests ${CSRCS} ${CPPSRCS} ${TEST_SRCS})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
add_test(NAME tests COMMAND tests)
