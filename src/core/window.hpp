#pragma once
#ifndef ANIMAFLOW_WINDOW_HPP
#define ANIMAFLOW_WINDOW_HPP

#include <string>
#include <vulkan/vulkan.h>

// Forward declaration
struct GLFWwindow;

namespace af::inline core {

    class Window {
    public:
        bool init(unsigned int width, unsigned int height, const std::string& title);
        bool initVulkan();
        void mainLoop();
        void cleanup();

    private:
        GLFWwindow* mWindow { nullptr };
        std::string mApplicationName;

        VkInstance mInstance{};
        VkSurfaceKHR mSurface{};
    };

}  // namespace af::inline core

#endif  // ANIMAFLOW_WINDOW_HPP
