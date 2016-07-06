include_directories(../src/c ../src/cpp)

set(CSRCS
	../src/c/radixsort.c
	)

set(CHDRS
	../src/c/helpers.h
	../src/c/radixsort.h
	)

set(CPPHDRS
	../src/cpp/helpers.hpp
	../src/cpp/radixsort.hpp
	../src/cpp/radixsort.inl
	)

set(SRCS
	#bench_radixsortc.cpp
	bench_radixsortcpp.cpp
	#main.cpp
	)

add_executable("runbench${CMAKE_MACHINE_TYPE}" ${CSRCS} ${CHDRS} ${CPPSRCS} ${CPPHDRS} ${SRCS})
