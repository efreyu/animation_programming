#pragma once
#ifndef ANIMAFLOW_WINDOW_INTERFACE_HPP
#define ANIMAFLOW_WINDOW_INTERFACE_HPP

#include <string>

namespace af::inline platform {
    class WindowInterface {
        virtual ~WindowInterface() = default;

        // Initialize the window and graphics context
        virtual bool init(unsigned int width, unsigned int height, const std::string& title) = 0;

        // Play the main loop
        virtual void mainLoop() = 0;

        // Cleanup resources
        virtual void cleanup() = 0;
    };
}  // namespace af::inline platform

#endif  // ANIMAFLOW_WINDOW_INTERFACE_HPP
