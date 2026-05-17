cmake_minimum_required(VERSION 3.20)

project(nt-call LANGUAGES CXX ASM)

add_library(${PROJECT_NAME} STATIC
    src/nt_syscall_bridge.S
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)
