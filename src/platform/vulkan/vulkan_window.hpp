#pragma once
#ifndef ANIMAFLOW_WINDOW_HPP
#define ANIMAFLOW_WINDOW_HPP

#include <string>
#include <platform/window_interface.hpp>
#include <vulkan/vulkan.h>

// Forward declaration
struct GLFWwindow;

namespace af::inline platform {

    class VulkanWindow : public WindowInterface {
    public:
        VulkanWindow() {};
        ~VulkanWindow() override {};
        bool init(unsigned int width, unsigned int height, const std::string& title) override;
        void mainLoop() override;
        void cleanup() override;

    private:
        bool initVulkan();
        void handleWindowMoveEvents(int xPos, int yPos);
        void handleWindowMinimizedEvents(int minimized);
        void handleWindowMaximizedEvents(int maximized);
        void handleWindowCloseEvents();

        GLFWwindow* mWindow { nullptr };
        std::string mApplicationName;

        VkInstance mInstance{};
        VkSurfaceKHR mSurface{};
    };

}  // namespace af::inline core

#endif  // ANIMAFLOW_WINDOW_HPP
