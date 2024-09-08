#pragma once
#ifndef ANIMAFLOW_FRAMEBUFFER_HPP
#define ANIMAFLOW_FRAMEBUFFER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace af::inline platform::opengl {
    class Framebuffer {
    public:
        bool init(unsigned int width, unsigned int height);
        bool resize(unsigned int newWidth, unsigned int newHeight);
        void bind();
        void unbind();
        void drawToScreen();
        void cleanup();

    private:
        unsigned int mBufferWidth = 640;
        unsigned int mBufferHeight = 480;
        GLuint mBuffer = 0;
        GLuint mColorTex = 0;
        GLuint mDepthBuffer = 0;

        bool checkComplete();
    };
}

#endif  // ANIMAFLOW_FRAMEBUFFER_HPP
