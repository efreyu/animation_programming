#pragma once
#ifndef ANIMAFLOW_CORE_MODULE_ATTRIBUTES_HPP
#define ANIMAFLOW_CORE_MODULE_ATTRIBUTES_HPP

namespace af::inline core {

#if defined(_MSC_VER)
        // MSVC doesn't have a clean way to ignore custom
        // attributes within a namespace like clang or gcc
#define _property
#define _signal_slot
#define _method
#elif defined(__GNUG__) || defined(__clang__)
        // these macros are used to define custom attributes to label godot signal callbacks
        // and node properties. they don't do anything other than making it easier to spot
        // these functions when reading the code or searching for them in an IDE.
#define _property    __attribute__((af("property")))
#define _signal_slot __attribute__((af("signal_slot")))
#define _method      __attribute__((af("method")))
#endif

}  // namespace af::inline utils

#endif  // ANIMAFLOW_CORE_MODULE_ATTRIBUTES_HPP
