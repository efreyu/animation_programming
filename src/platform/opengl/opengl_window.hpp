#pragma once
#ifndef ANIMAFLOW_OPENGL_WINDOW_HPP
#define ANIMAFLOW_OPENGL_WINDOW_HPP

#include <string>
#include <memory>
#include "platform/window_interface.hpp"
#include "model/model.hpp"
#include "render/ogl_renderer.hpp"

// Forward declaration
struct GLFWwindow;

namespace af::inline platform::opengl {

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

        std::unique_ptr<OGLRenderer> mRenderer;
        std::unique_ptr<Model> mModel;

        unsigned int mScreenWidth = 640;
        unsigned int mScreenHeight = 480;

    };

}  // namespace af::inline core

#endif  // ANIMAFLOW_OPENGL_WINDOW_HPP
