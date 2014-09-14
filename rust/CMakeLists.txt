cmake_minimum_required(VERSION 2.8.3)
project(radixsort NONE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
find_package(rustc)
find_package(rustdoc)
include(Rust)

if ("${CMAKE_MACHINE_TYPE}" EQUAL "32")
           set(ARCH_FLAGS -C target-cpu=i686)
elseif ("${CMAKE_MACHINE_TYPE}" EQUAL "64")
           set(ARCH_FLAGS -C target-cpu=x86-64)
else()
           message(FATAL_ERROR "Unknown CMAKE_MACHINE_TYPE '${CMAKE_MACHINE_TYPE}'")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
           set(RUSTC_FLAGS -g --opt-level=0 ${ARCH_FLAGS})
           set(TARGET_SUFFIX "${CMAKE_MACHINE_TYPE}d")
else()
           set(RUSTC_FLAGS -g --opt-level=3 ${ARCH_FLAGS})
           set(TARGET_SUFFIX "${CMAKE_MACHINE_TYPE}")
endif()

set(PERF_TARGET "radixperfrs${TARGET_SUFFIX}")
set(TEST_TARGET "radixtestrs${TARGET_SUFFIX}")

get_rust_deps(perf.rs PERF_DEPS)
get_rust_deps(test.rs TEST_DEPS)

rust_crate(perf.rs
           TARGET_NAME "${PERF_TARGET}"
           DEPENDS "${PERF_DEPS}"
           OTHER_RUSTC_FLAGS -o "${PERF_TARGET}")
rust_crate(test.rs
           TARGET_NAME "${TEST_TARGET}"
           DEPENDS "${TEST_DEPS}"
           OTHER_RUSTC_FLAGS -o "${TEST_TARGET}")

set(PERF_FULL_TARGET "${PERF_TARGET}_FULL_TARGET")
set(TEST_FULL_TARGET "${TEST_TARGET}_FULL_TARGET")

add_custom_target(radixall
                  ALL
                  DEPENDS ${${PERF_FULL_TARGET}} ${${TEST_FULL_TARGET}})

