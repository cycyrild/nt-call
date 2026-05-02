cmake_minimum_required(VERSION 3.20)

project(nt-call LANGUAGES CXX ASM)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cc
    src/*.S
)

add_library(nt-call STATIC
    ${SRC_FILES}
)

add_library(nt-call::nt-call ALIAS nt-call)

target_include_directories(nt-call
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
