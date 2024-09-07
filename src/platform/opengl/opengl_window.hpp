#pragma once
#ifndef ANIMAFLOW_OPENGL_WINDOW_HPP
#define ANIMAFLOW_OPENGL_WINDOW_HPP

#include <string>
#include "platform/window_interface.hpp"

// Forward declaration
struct GLFWwindow;

namespace af::inline platform {

    class OpenGLWindow : public WindowInterface {
    public:
        OpenGLWindow() {};
        ~OpenGLWindow() override {};

        bool init(unsigned int width, unsigned int height, const std::string& title) override;
        void mainLoop() override;
        void cleanup() override;

    private:
        void handleWindowMoveEvents(int x, int y);
        void handleWindowMinimizedEvents(int minimized);
        void handleWindowMaximizedEvents(int maximized);
        void handleWindowCloseEvents();

        void handleKeyEvents(int key, int scancode, int action, int mods);
        void handleMouseButtonEvents(int button, int action, int mods);
        void handleMousePositionEvents(double x, double y);
        void handleMouseEnterLeaveEvents(int enter);

        GLFWwindow* mWindow { nullptr };
        std::string mApplicationName;

    };

}  // namespace af::inline core

#endif  // ANIMAFLOW_OPENGL_WINDOW_HPP
