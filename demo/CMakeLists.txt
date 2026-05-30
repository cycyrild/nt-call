cmake_minimum_required(VERSION 3.20)

project(
    nt-call-demo
    LANGUAGES CXX
)
include(nt-crtless-runtime/cmake/gnu-link.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/nt-call")
add_subdirectory(nt-crtless-runtime)

add_executable(${PROJECT_NAME}
    main.cc
    printf.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    nt-call
    nt-crtless-runtime
)

target_link_options(${PROJECT_NAME} PRIVATE
    "LINKER:/entry:start"
    "LINKER:/map:$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.map"
    "LINKER:/opt:ref"
    "LINKER:/opt:icf"
    "LINKER:/timestamp:0"
    "LINKER:/lldsavetemps"
)

add_gnu_final_link(${PROJECT_NAME} nt-call)
