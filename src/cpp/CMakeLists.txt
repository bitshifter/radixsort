set(CPPHDRS
	helpers.hpp
	radixsort.hpp
	radixsort.inl
	)

add_executable("radixperfcpp${CMAKE_MACHINE_TYPE}" ${CPPHDRS} perf.cpp)
add_executable("radixtestcpp${CMAKE_MACHINE_TYPE}" ${CPPHDRS} test.cpp)
