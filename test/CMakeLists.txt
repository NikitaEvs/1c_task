cmake_minimum_required(VERSION 3.16)

set(googletest_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-src)
set(googletest_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY    https://github.com/google/googletest.git
        GIT_TAG           master
        SOURCE_DIR        "${googletest_SOURCE_DIR}"
        BINARY_DIR        "${googletest_SOURCE_DIR}"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(unit_test tests.cpp)

target_link_libraries(
        unit_test
        core
        gtest
        gtest_main
        pthread
)
