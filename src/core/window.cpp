#include "window.hpp"
#include "core/assert.hpp"
#include <GLFW/glfw3.h>

using namespace af;

bool Window::init(unsigned int width, unsigned int height, const std::string& title) {
    if (!glfwInit()) {
        error_msg("glfwInit() error");
        return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!mWindow) {
        error_msg_format("Could not create window: {}", title.c_str());
        glfwTerminate();
        return false;
    }

//    Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);
    return true;
}

void Window::mainLoop() {
    while (!glfwWindowShouldClose(mWindow)) {

        /* poll events in a loop */
        glfwPollEvents();

    }
}

void Window::cleanup() {
//    Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}