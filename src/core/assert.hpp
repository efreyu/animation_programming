#pragma once
#ifndef ANIMAFLOW_CORE_MODULE_ASSERT_HPP
#define ANIMAFLOW_CORE_MODULE_ASSERT_HPP

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <format>

#ifndef NDEBUG
#if defined(__GNUC__) || defined(__clang__)
#define GENERATE_TRAP() __builtin_trap()  // GCC/Clang specific trap
#elif defined(_MSC_VER)
#include <intrin.h>
#define GENERATE_TRAP() __debugbreak()    // MSVC specific trap
#else
#define GENERATE_TRAP() raise(SIGTRAP)    // Portable alternative using SIGTRAP
#endif
//
// In debug mode, checks the passed in condition and outputs
// detailed information to stederr, including a custom error
// message when the condition evaluates to false.
//
#define assertion(condition, message)                                               \
    do {                                                                            \
        if (!(condition)) [[unlikely]] {                                            \
            std::cerr << __FUNCTION__ << __FILE__ << __LINE__ <<                    \
                                      message " => condition: (" #condition ")";    \
            std::cerr.flush();                                                      \
            GENERATE_TRAP();                                                        \
        }                                                                           \
    } while (false)

#define error_msg(message)                                                          \
    do {                                                                            \
        std::cerr << __FUNCTION__ << __FILE__ << __LINE__ << message;               \
        std::cerr.flush();                                                          \
        GENERATE_TRAP();                                                            \
    } while (false)
#define error_msg_format(format_str, ...)                                           \
    do {                                                                            \
        std::string formatted_msg = std::format(format_str, __VA_ARGS__);           \
        error_msg(formatted_msg);                                                   \
    } while (false)

//#define info_msg_base(message)                                                                    \
//    do {                                                                                          \
//        ::godot::UtilityFunctions::print("[INFO] (", __FUNCTION__, ") ", __FILE__, ":", __LINE__, \
//                                         " ", message);                                           \
//    } while (false)
//#define info_msg(...) info_msg_base(godot::UtilityFunctions::str(__VA_ARGS__))

#define runtime_assert(condition) assertion(condition, "validation check failed")

#else
//
// In release mode the macro does nothing ((void)0), including
// the execution of the condition so don't define the expression
// as anything that would be considered program logis.
//
#define assertion(condition, message)       static_cast<void>(0)
#define error_msg(message)                  static_cast<void>(0)
#define error_msg_format(format_str, ...)   static_cast<void>(0)
//#define info_msg_base(message)              static_cast<void>(0)
//#define info_msg(...)                       static_cast<void>(0)
#define runtime_assert(condition)           static_cast<void>(0)
#endif

#endif  // ANIMAFLOW_CORE_MODULE_ASSERT_HPP
