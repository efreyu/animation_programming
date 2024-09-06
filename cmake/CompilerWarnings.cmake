string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE )

if ( NOT MSVC )
    option( ${PROJECT_NAME_UPPERCASE}_WARN_EVERYTHING "Turn on all warnings (not recommended - used for lib development)" OFF )
endif()

option( ${PROJECT_NAME_UPPERCASE}_WARNING_AS_ERROR "Treat warnings as errors" ON )

# Add warnings based on compiler
# Set some helper variables for readability
set(compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>")
set(compiler_is_gnu "$<CXX_COMPILER_ID:GNU>")
set(compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>")

target_compile_options(${PROJECT_NAME} PUBLIC
        # MSVC only
        $<${compiler_is_msvc}:
        /EHsc
        /utf-8
        /Zc:preprocessor
        $<$<CONFIG:Debug>:
        /MDd
        >
        $<$<CONFIG:Release>:
        /MD
        /O2
        /W4
        >
        >
        # Clang and GNU
        $<$<NOT:${compiler_is_msvc}>:
        -g
        -Wall
        -Wno-unused-value
        $<${compiler_is_gnu}:
        -Wno-attributes
        -Wno-attributes=bt::
        >
        # Clang only
        $<${compiler_is_clang}:
        -Wno-ignored-attributes
        -Wno-unknown-attributes
        >
        $<$<CONFIG:Debug>:
        -fno-omit-frame-pointer
        -O0
        >
        $<$<CONFIG:Release>:
        -O3
        >
        >
)

# Turn on (almost) all warnings on Clang, Apple Clang, and GNU.
# Useful for internal development, but too noisy for general development.
function( set_warn_everything )
    message( STATUS "[${PROJECT_NAME}] Turning on (almost) all warnings")

    target_compile_options( ${PROJECT_NAME}
        PRIVATE
            # Clang and GNU
            $<$<OR:${compiler_is_clang},${compiler_is_gnu}>:
                -Weverything
                -Wno-c++98-compat
                -Wno-c++98-compat-pedantic
                -Wno-padded
            >
    )
endfunction()

if ( NOT MSVC AND ${PROJECT_NAME_UPPERCASE}_WARN_EVERYTHING )
    set_warn_everything()
endif()

# Treat warnings as errors
function( set_warning_as_error )
    message( STATUS "[${PROJECT_NAME}] Treating warnings as errors")

    if ( CMAKE_VERSION VERSION_GREATER_EQUAL "3.24" )
        set_target_properties( ${PROJECT_NAME}
            PROPERTIES
                COMPILE_WARNING_AS_ERROR ON
        )
    else()
        target_compile_options( ${PROJECT_NAME}
            PRIVATE
            $<${compiler_is_msvc}:/WX>
            $<$<OR:${compiler_is_clang},${compiler_is_gnu}>:-Werror>
        )
    endif()
endfunction()

if ( ${PROJECT_NAME_UPPERCASE}_WARNING_AS_ERROR )
    set_warning_as_error()
endif()
