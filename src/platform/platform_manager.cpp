#include "platform/window_interface.hpp"
#ifdef USE_VULKAN
#include "platform/vulkan/vulkan_window.hpp"
#else
#include "platform/opengl/opengl_window.hpp"
#endif
#include "platform_manager.hpp"
#include "core/assert.hpp"
#include <memory>

using namespace af;

PlatformManager* PlatformManager::pInstance = nullptr;
bool PlatformManager::destroyed = false;

PlatformManager& PlatformManager::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void PlatformManager::cleanup() {
    destroyed = true;
    pInstance = nullptr;
}

void PlatformManager::create() {
    static PlatformManager instance;
    pInstance = &instance;
}

void PlatformManager::onDeadReference() {
    assertion(false, "Founded dead reference!");
}

PlatformManager::~PlatformManager() {
}

std::unique_ptr<WindowInterface> PlatformManager::createWindow() {
    if (!mWindow) {
#ifdef USE_VULKAN
        mWindow = std::make_unique<VulkanWindow>();
#else
        mWindow = std::make_unique<OpenGLWindow>();
#endif
    }
    return std::move(mWindow);
}