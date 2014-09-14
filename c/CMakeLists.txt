set(CSRCS
	radixsort.c
	)

set(CHDRS
	helpers.h
	radixsort.h
	timer.h
	)

if(WIN32)
	list(APPEND CSRCS timer_impl_windows.c)
endif()

if(UNIX AND NOT APPLE)
	list(APPEND CSRCS timer_impl_linux.c)
endif()

add_executable("radixperfc${CMAKE_MACHINE_TYPE}" ${CSRCS} ${CHDRS} perf.c)
add_executable("radixtestc${CMAKE_MACHINE_TYPE}" ${CSRCS} ${CHDRS} test.c)
