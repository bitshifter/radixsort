cmake_minimum_required(VERSION 3.12)

project(radixsort VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerAndLinkerOptions)

include(GNUInstallDirs)
include(CTest)

add_subdirectory(thirdparty/Catch2)

# C shared library ------------------------------------------------------------
add_library(radixsort_c SHARED
    src/c/radixsort.c
    src/c/radixsort.h
)
target_include_directories(radixsort_c PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/c>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(radixsort_c PROPERTIES
    VERSION     ${PROJECT_VERSION}
    SOVERSION   ${PROJECT_VERSION_MAJOR}
    C_VISIBILITY_PRESET hidden
)

# Executables -----------------------------------------------------------------
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
target_compile_definitions(bench PRIVATE RADIXSORT_C_STATIC)

set(TEST_SRCS
	test/test_radixsortcpp.cpp
	test/test_radixsortc.cpp
	)

add_executable(tests ${CSRCS} ${CPPSRCS} ${TEST_SRCS})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_definitions(tests PRIVATE RADIXSORT_C_STATIC)
add_test(NAME tests COMMAND tests)

# Install ---------------------------------------------------------------------
install(TARGETS radixsort_c
    EXPORT radixsort_c-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES src/c/radixsort.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT radixsort_c-targets
    FILE radixsort_cTargets.cmake
    NAMESPACE radixsort::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/radixsort_c
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/radixsort_cConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/radixsort_cConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/radixsort_c
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/radixsort_cConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/radixsort_cConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/radixsort_cConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/radixsort_c
)
