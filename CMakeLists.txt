cmake_minimum_required( VERSION 3.23 )

set(PROJECT_NAME AnimaFlow)
set(PROJECT_VERSION 0.1.0)

message( STATUS "Using CMake ${CMAKE_VERSION}" )

# Require out-of-source builds
file( TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH )

if ( EXISTS "${LOC_PATH}" )
    message( FATAL_ERROR "You cannot build in the source directory. Please use a build subdirectory." )
endif()

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
add_subdirectory(thirdparty/glfw EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/glew EXCLUDE_FROM_ALL)

#add_subdirectory(thirdparty/fmt)
#add_subdirectory(thirdparty/magic_enum)

# =======================================================================
# Dependency linkage
# =======================================================================

target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw
        PRIVATE libglew_static
#        PRIVATE fmt::fmt
#        PRIVATE fmt::fmt-header-only
#        PRIVATE spdlog::spdlog_header_only
#        PRIVATE magic_enum::magic_enum
)
