#pragma once
#ifndef ANIMAFLOW_PLATFORM_MANAGER_HPP
#define ANIMAFLOW_PLATFORM_MANAGER_HPP

#include "platform/window_interface.hpp"
#include <memory>

namespace af::inline platform {

    class PlatformManager {
    public:
        static PlatformManager& getInstance();
        static void cleanup();

        // Create the window (Vulkan, OpenGL, etc.)
        std::unique_ptr<WindowInterface> createWindow();

    private:
        PlatformManager() = default;
        ~PlatformManager();
        PlatformManager(const PlatformManager&) = delete;
        PlatformManager& operator=(const PlatformManager&) = delete;
        static void create();
        static void onDeadReference();

        static PlatformManager* pInstance;
        static bool destroyed;

        std::unique_ptr<WindowInterface> mWindow;

    };
}  // namespace af::inline platform

#endif  // ANIMAFLOW_PLATFORM_MANAGER_HPP
