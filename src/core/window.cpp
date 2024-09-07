#include "window.hpp"

#include "core/assert.hpp"

#include <GLFW/glfw3.h>

using namespace af;

/*bool Window::init(unsigned int width, unsigned int height, const std::string& title) {
    if (!glfwInit()) {
        error_msg("glfwInit() error");
        return false;
    }

    if (!glfwVulkanSupported()) {
        glfwTerminate();
        error_msg("Vulkan is not supported");
        return false;
    }

    mApplicationName = title;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    *//* Vulkan needs no context *//*
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    mWindow = glfwCreateWindow(width, height, mApplicationName.c_str(), nullptr, nullptr);

    if (!mWindow) {
        error_msg_format("Could not create window: {}", title.c_str());
        glfwTerminate();
        return false;
    }

    if (!initVulkan()) {
        error_msg("Could not init Vulkan");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    info_msg("Window successfully initialized");
    return true;
}*/

bool Window::init(unsigned int width, unsigned int height, const std::string& title) {
    if (!glfwInit()) {
        error_msg("glfwInit() error");
        return false;
    }

    /* set a "hint" for the NEXT window created*/
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!mWindow) {
        error_msg("Could not create window");
        glfwTerminate();
        return false;
    }

    /* the C handlers needs a little 'stunt' here */
    /* 1) save the pointer to the instance as user pointer */
    glfwSetWindowUserPointer(mWindow, this);

    /* 2) use a lambda to get the pointer and call the member function */
    glfwSetWindowPosCallback(mWindow, [](GLFWwindow *win, int xpos, int ypos) {
        auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowMoveEvents(xpos, ypos);
    }
    );

    glfwSetWindowIconifyCallback(mWindow, [](GLFWwindow *win, int minimized) {
        auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowMinimizedEvents(minimized);
    }
    );

    glfwSetWindowMaximizeCallback(mWindow, [](GLFWwindow *win, int maximized) {
        auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowMaximizedEvents(maximized);
    }
    );

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow *win) {
        auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowCloseEvents();
    }
    );

    info_msg("Window successfully initialized");
    return true;
}

bool Window::initVulkan() {
    VkResult result = VK_ERROR_UNKNOWN;

    VkApplicationInfo mAppInfo{};
    mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    mAppInfo.pNext = nullptr;
    mAppInfo.pApplicationName = mApplicationName.c_str();
    mAppInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
    mAppInfo.pEngineName = "AnimaFlow: Game Animations Programming";
    mAppInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    mAppInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 1, 0);

    uint32_t extensionCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<const char*> extNames;
#ifdef __APPLE__
    extNames.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    extNames.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

    if (extensionCount == 0) {
        error_msg("No Vulkan extensions found, need at least 'VK_KHR_surface'");
        return false;
    }

    info_msg_format("Found {} Vulkan extensions:", extensionCount);
    for (int i = 0; i < extensionCount; ++i) {
        info_msg_format(" - {}", extensions[i]);
        extNames.push_back(extensions[i]);
    }

    VkInstanceCreateInfo mCreateInfo{};
    mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    mCreateInfo.pNext = nullptr;
    mCreateInfo.pApplicationInfo = &mAppInfo;
    mCreateInfo.enabledLayerCount = 0;
#ifdef __APPLE__
    mCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
    mCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extNames.size());
    mCreateInfo.ppEnabledExtensionNames = extNames.data();


    result = vkCreateInstance(&mCreateInfo, nullptr, &mInstance);
    if (result != VK_SUCCESS) {
        error_msg_format("Could not create Vulkan instance: {}", static_cast<int>(result));
        return false;
    }

    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0) {
        error_msg("No Vulkan capable GPU found");
        return false;
    }

    std::vector<VkPhysicalDevice> devices;
    devices.resize(physicalDeviceCount);
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, devices.data());

    info_msg_format("Found {} physical device(s)", physicalDeviceCount);

    result = glfwCreateWindowSurface(mInstance, mWindow, nullptr, &mSurface);
    if (result != VK_SUCCESS) {
        error_msg("Could not create Vulkan surface");
        return false;
    }

    return true;
}

void Window::mainLoop() {
    // force VSYNC
    //    glfwSwapInterval(1);

    //    float color = 0.0f;
    while (!glfwWindowShouldClose(mWindow)) {
        //        color >= 1.0f ? color = 0.0f : color += 0.01f;
        //
        //        glClearColor(color, color, color, 1.0f);
        //        glClear(GL_COLOR_BUFFER_BIT);
        //
        //        /* swap buffers */
        //        glfwSwapBuffers(mWindow);

        /* poll events in a loop */
        glfwPollEvents();
    }
}

void Window::cleanup() {
    info_msg("Cleaning up window");

//    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
//    vkDestroyInstance(mInstance, nullptr);

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Window::handleWindowMoveEvents(int xPos, int yPos) {
    info_msg_format("Window moved to: ({}, {})", xPos, yPos);
}

void Window::handleWindowMinimizedEvents(int minimized) {
    if (minimized) {
        info_msg("Window minimized");
    } else {
        info_msg("Window restored");
    }
}

void Window::handleWindowMaximizedEvents(int maximized) {
    if (maximized) {
        info_msg("Window maximized");
    } else {
        info_msg("Window restored");
    }
}

void Window::handleWindowCloseEvents() {
    info_msg("Window close event received");
}