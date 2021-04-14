cmake_minimum_required(VERSION 3.16)

project(1c_task CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -g")

if (CHECK_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif(CHECK_COVERAGE)

set(UBSAN_COMPILE_FLAGS -fsanitize=undefined -fno-sanitize-recover=all)
set(UBSAN_LINK_FLAGS -fsanitize=undefined)

set(ASAN_COMPILE_FLAGS -fsanitize=address,undefined -fno-sanitize-recover=all)
set(ASAN_LINK_FLAGS -fsanitize=address,undefined)


include_directories(
        thirdparty
        thirdparty/json
        core
        src
)

add_subdirectory(core)

add_subdirectory(thirdparty)

if (TEST_BUILD)
    add_subdirectory(test)
endif(TEST_BUILD)

if (UBSAN_SANITIZE_BUILD)
    add_compile_options(${UBSAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${UBSAN_LINK_FLAGS}")
endif(UBSAN_SANITIZE_BUILD)

if (ASAN_SANITIZE_BUILD)
    add_compile_options(${ASAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
endif(ASAN_SANITIZE_BUILD)

add_executable(main src/main.cpp)
target_link_libraries(main
        core
        json)

#install(TARGETS main DESTINATION bin)
