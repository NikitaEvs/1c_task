cmake_minimum_required(VERSION 3.16)

set (JSON_PATH json)

set (JSON_SOURCES
        ${JSON_PATH}/json.hpp
        )

add_library(
        json SHARED
        ${JSON_SOURCES}
)

set_target_properties(json PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
