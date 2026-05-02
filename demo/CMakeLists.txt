cmake_minimum_required(VERSION 3.20)

project(
    nt-call-demo
    LANGUAGES C CXX ASM
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/nt-call")
add_subdirectory(runtime)

add_executable(demo
    main.cc
    printf.cc
)

target_link_libraries(demo PRIVATE
    nt-call::nt-call
    freestanding-runtime
)

target_link_options(demo PRIVATE
    -Wl,-e,start
    -Wl,-Map,demo.map
    -Wl,--gc-sections
    -Wl,--no-insert-timestamp
    -Wl,--exclude-all-symbols
    -Wl,--strip-all
)
