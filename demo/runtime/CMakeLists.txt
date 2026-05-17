cmake_minimum_required(VERSION 3.20)

block()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(ALIAS_STANDARD_FUNCTION_NAMES HARD CACHE STRING "" FORCE)

    add_subdirectory(printf)
endblock()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cc
    src/*.S
)

add_library(freestanding-runtime STATIC
    ${SRC_FILES}
)

target_compile_definitions(freestanding-runtime
    PRIVATE
        WIN32_LEAN_AND_MEAN
)

target_include_directories(freestanding-runtime
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_include_directories(freestanding-runtime
    SYSTEM PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include_nt"
)

target_link_libraries(freestanding-runtime
    PUBLIC
        printf::printf
)

target_compile_options(printf PRIVATE
    -Wno-everything
)
