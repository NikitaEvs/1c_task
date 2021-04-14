cmake_minimum_required(VERSION 3.16)

set (FIELD_PATH Field)

set (FIELD_SOURCES
        ${FIELD_PATH}/core.hpp
        )

add_library(
        core SHARED
        ${FIELD_SOURCES}
)

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
