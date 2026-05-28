cmake_minimum_required(VERSION 3.20)

project(
    nt-call-demo
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/nt-call")
add_subdirectory(runtime)

separate_arguments(NT_CALL_DEMO_STANDARD_LIBRARIES NATIVE_COMMAND "${CMAKE_CXX_STANDARD_LIBRARIES}")
list(REMOVE_ITEM NT_CALL_DEMO_STANDARD_LIBRARIES -loldnames)

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND
        sh "${CMAKE_CURRENT_LIST_DIR}/cmake/gnu-final-link.sh"
            "$<TARGET_FILE:${PROJECT_NAME}>"
            "$<TARGET_FILE:nt-call>"
            ${NT_CALL_DEMO_STANDARD_LIBRARIES}
    BYPRODUCTS
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.ld.exe"
    VERBATIM
)
