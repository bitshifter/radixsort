include_directories(../src/c ../src/cpp)

if(NOT MSVC)
	set(CSRCS
		../src/c/radixsort.c
		../src/c/radixsort.h
		)
endif()

set(CPPSRCS
	../src/cpp/radixsort.hpp
	../src/cpp/radixsort.inl
	../thirdparty/nonius/nonius.hpp
	)

set(SRCS
	main.cpp
	)

# nonius requires boost and threads
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(runbench ${CSRCS} ${CPPSRCS} ${SRCS})
target_link_libraries(runbench PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(runbench PRIVATE ../thirdparty/nonius ${Boost_INCLUDE_DIRS})
