cmake_minimum_required( VERSION 3.23 )

set(PROJECT_NAME AnimaFlow)
set(PROJECT_VERSION 0.1.0)

message( STATUS "Using CMake ${CMAKE_VERSION}" )

# Require out-of-source builds
file( TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH )

if ( EXISTS "${LOC_PATH}" )
    message( FATAL_ERROR "You cannot build in the source directory. Please use a build subdirectory." )
endif()

# CMake option to switch between OpenGL and Vulkan
option(USE_VULKAN "Use Vulkan for rendering" OFF)

# Add paths to modules
list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

# Turn on link time optimization for everything
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )

# Output compile commands to compile_commands.json (for debugging CMake issues)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Build universal lib on macOS
# Note that CMAKE_OSX_ARCHITECTURES must be set before project().
if ( APPLE )
    set( CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" )
endif()

# Main project information
project( ${PROJECT_NAME}
        LANGUAGES
        C CXX
        VERSION
        ${PROJECT_VERSION}
)

# Include helper functions
include( helpers )
set(GAME_SOURCE)

# add sources
add_subdirectory(src/core)
add_subdirectory(src/platform)

# add th app executable
add_executable(${PROJECT_NAME} ${GAME_SOURCE})

# include directories gdextension library
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_features( ${PROJECT_NAME}
        PRIVATE
        cxx_std_23
)

if( NOT DEFINED CMAKE_DEBUG_POSTFIX )
    set_target_properties( ${PROJECT_NAME}
            PROPERTIES
            DEBUG_POSTFIX "-d"
    )
endif()

# Warnings
include( CompilerWarnings )

# Create and include version info file from git
include( GitVersionInfo )

if (NOT APPLE)
    # linker options for the gdextension library
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<NOT:${compiler_is_msvc}>:
            -static-libgcc
            -static-libstdc++
            -Wl,-R,'$$ORIGIN'
            >
    )
endif ()

# ccache
# Turns on ccache if found
include( ccache )

# Formatting
# Adds a custom target to format all the code at once
include( ClangFormat )

# =======================================================================
# 3rd party library setup/configuration
# =======================================================================

add_definitions(-DGLEW_STATIC)
add_definitions(-DGL_SILENCE_DEPRECATION)
add_subdirectory(thirdparty/glfw EXCLUDE_FROM_ALL)

set(RENDER_LIBS)

# Find OpenGL or Vulkan based on the selected mode
if(USE_VULKAN)
    if(NOT DEFINED ENV{VULKAN_SDK})
        message(FATAL_ERROR "VULKAN_SDK environment variable is not set. Please install Vulkan SDK and set the environment variable.")
    endif()
    find_package(Vulkan REQUIRED)
    set(RENDER_LIBS ${RENDER_LIBS} Vulkan::Vulkan)
    add_definitions(-DUSE_VULKAN) # Add a definition to the project
else()
    add_subdirectory(thirdparty/glad)
    find_package(OpenGL REQUIRED)
    find_package(glm REQUIRED)
    include_directories(${GLM_INCLUDE_DIRS})
    set(RENDER_LIBS ${RENDER_LIBS} OpenGL::GL glad)
endif()


#add_subdirectory(thirdparty/fmt)
#add_subdirectory(thirdparty/magic_enum)

# =======================================================================
# Dependency linkage
# =======================================================================

target_link_libraries(${PROJECT_NAME}
        PRIVATE ${RENDER_LIBS}
        PRIVATE glfw
#        PRIVATE fmt::fmt
#        PRIVATE fmt::fmt-header-only
#        PRIVATE spdlog::spdlog_header_only
#        PRIVATE magic_enum::magic_enum
)


# copy shader files
file(GLOB GLSL_SOURCE_FILES
        shader/*.frag
        shader/*.vert
)
add_custom_target(
        Shaders
        DEPENDS ${GLSL_SOURCE_FILES}
)
add_dependencies(${PROJECT_NAME} Shaders)
add_custom_command(TARGET Shaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:${PROJECT_NAME},SOURCE_DIR>/shader"
        "$<TARGET_PROPERTY:${PROJECT_NAME},BINARY_DIR>/$<CONFIGURATION>/shader"
)
# copy textures
file(GLOB TEX_SOURCE_FILES
        assets/*
)
add_custom_target(
        Textures
        DEPENDS ${TEX_SOURCE_FILES}
)
add_dependencies(${PROJECT_NAME} Textures)

add_custom_command(TARGET Textures POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:${PROJECT_NAME},SOURCE_DIR>/assets"
        "$<TARGET_PROPERTY:${PROJECT_NAME},BINARY_DIR>/$<CONFIGURATION>/assets"
)