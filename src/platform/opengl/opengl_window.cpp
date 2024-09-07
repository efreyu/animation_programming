#include "opengl_window.hpp"

#include "core/assert.hpp"

#include <GLFW/glfw3.h>

using namespace af;

bool OpenGLWindow::init(unsigned int width, unsigned int height, const std::string& title) {
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
    glfwSetWindowPosCallback(mWindow, [](GLFWwindow* win, int xpos, int ypos) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowMoveEvents(xpos, ypos);
    });

    glfwSetWindowIconifyCallback(mWindow, [](GLFWwindow* win, int minimized) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowMinimizedEvents(minimized);
    });

    glfwSetWindowMaximizeCallback(mWindow, [](GLFWwindow* win, int maximized) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowMaximizedEvents(maximized);
    });

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* win) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowCloseEvents();
    });

    info_msg("Window successfully initialized");
    return true;
}

void OpenGLWindow::mainLoop() {
    // force VSYNC
    glfwSwapInterval(1);

    float color = 0.0f;
    while (!glfwWindowShouldClose(mWindow)) {
        color >= 1.0f ? color = 0.0f : color += 0.01f;

        glClearColor(color, color, color, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* swap buffers */
        glfwSwapBuffers(mWindow);

        /* poll events in a loop */
        glfwPollEvents();
    }
}

void OpenGLWindow::cleanup() {
    info_msg("Cleaning up window");

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void OpenGLWindow::handleWindowMoveEvents(int xPos, int yPos) {
    info_msg_format("Window moved to: ({}, {})", xPos, yPos);
}

void OpenGLWindow::handleWindowMinimizedEvents(int minimized) {
    if (minimized)
        info_msg("Window minimized");
    else
        info_msg("Window restored");
}

void OpenGLWindow::handleWindowMaximizedEvents(int maximized) {
    if (maximized)
        info_msg("Window maximized");
    else
        info_msg("Window restored");
}

void OpenGLWindow::handleWindowCloseEvents() {
    info_msg("Window close event received");
}