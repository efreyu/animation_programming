cmake_minimum_required(VERSION 3.6)

set(lib_name glad)

project(${lib_name})

list(APPEND GLAD_SOURCES
        glad/glad.h
        KHR/khrplatform.h
        glad.c
        stb_image.h
)

add_library(${lib_name} STATIC ${GLAD_SOURCES})

target_include_directories(${lib_name} PUBLIC .)

set_target_properties(${lib_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

