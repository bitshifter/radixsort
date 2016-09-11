include_directories(../src/c ../src/cpp ../thirdparty/Catch)

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
	test_radixsortc.cpp
	test_radixsortcpp.cpp
	main.cpp
	)

# Catch requires exceptions and rtti
if(MSVC)
	add_compile_options()
else()
	add_compile_options(-fexceptions -frtti)
endif()

add_executable("runtests${CMAKE_MACHINE_TYPE}" ${CSRCS} ${CHDRS} ${CPPSRCS} ${CPPHDRS} ${SRCS})
