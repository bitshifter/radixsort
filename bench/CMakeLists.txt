include_directories(../src/c ../src/cpp)

set(CSRCS
	../src/c/radixsort.c
	)

set(CHDRS
	../src/c/radixsort.h
	)

set(CPPHDRS
	../src/cpp/radixsort.hpp
	../src/cpp/radixsort.inl
	)

set(SRCS
	main.cpp
	)

# nonius requires boost and threads
find_package(Boost)
find_package(Threads)

# nonius requires exceptions
if(MSVC)
	# TODO
	add_compile_options()
else()
	add_compile_options(-fexceptions)
endif()

add_executable(runbench ${CSRCS} ${CHDRS} ${CPPSRCS} ${CPPHDRS} ${SRCS})
target_link_libraries(runbench PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(runbench PRIVATE ../thirdparty/nonius)
