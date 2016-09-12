include_directories(../src/c ../src/cpp ../thirdparty/Catch)

if(NOT MSVC)
	set(CSRCS
		../src/c/radixsort.c
		../src/c/radixsort.h
		test_radixsortc.cpp
		)
endif()

set(CPPSRCS
	../src/cpp/radixsort.hpp
	../src/cpp/radixsort.inl
	test_radixsortcpp.cpp
	main.cpp
	)

add_executable("runtests" ${CSRCS} ${CPPSRCS})
