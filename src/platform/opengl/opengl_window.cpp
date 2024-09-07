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

    glfwSetKeyCallback(mWindow, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleKeyEvents(key, scancode, action, mods);
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *win, int button, int action, int mods) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleMouseButtonEvents(button, action, mods);
    });

    glfwSetCursorPosCallback(mWindow, [](GLFWwindow *win, double x, double y) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleMousePositionEvents(x, y);
    });

    glfwSetCursorEnterCallback(mWindow, [](GLFWwindow *win, int enter) {
        auto thisWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(win));
        thisWindow->handleMouseEnterLeaveEvents(enter);
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

void OpenGLWindow::handleWindowMoveEvents(int x, int y) {
    info_msg_format("Window moved to: ({}, {})", x, y);
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

void OpenGLWindow::handleKeyEvents(int key, int scancode, int action, int mods) {
    std::string actionName;
    switch (action) {
        case GLFW_PRESS:
            actionName = "pressed";
            break;
        case GLFW_RELEASE:
            actionName = "released";
            break;
        case GLFW_REPEAT:
            actionName = "repeated";
            break;
        default:
            actionName = "invalid";
            break;
    }

    const char *keyName = glfwGetKeyName(key, 0);
    info_msg_format("key {} (key {}, scancode {}) {}", keyName ? keyName : "null", key, scancode, actionName);
}

void OpenGLWindow::handleMouseButtonEvents(int button, int action, int mods) {
    std::string actionName;
    switch (action) {
        case GLFW_PRESS:
            actionName = "pressed";
            break;
        case GLFW_RELEASE:
            actionName = "released";
            break;
        case GLFW_REPEAT:
            actionName = "repeated";
            break;
        default:
            actionName = "invalid";
            break;
    }

    std::string mouseButtonName;
    switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButtonName = "left";
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButtonName = "middle";
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseButtonName = "right";
            break;
        default:
            mouseButtonName = "other";
            break;
    }
    info_msg_format("{} mouse button ({}) {}", mouseButtonName, button, actionName);
}

void OpenGLWindow::handleMousePositionEvents(double x, double y) {
    info_msg_format("Mouse position: ({}, {})", x, y);
}

void OpenGLWindow::handleMouseEnterLeaveEvents(int enter) {
    if (enter)
        info_msg("Mouse entered window");
    else
        info_msg("Mouse left window");
}
