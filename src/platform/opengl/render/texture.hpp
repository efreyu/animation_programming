#pragma once
#ifndef ANIMAFLOW_TEXTURE_HPP
#define ANIMAFLOW_TEXTURE_HPP
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace af::inline platform::opengl {
    class Texture {
    public:
        bool loadTexture(const std::string& textureFilename);
        void bind();
        void unbind();
        void cleanup();

    private:
        GLuint mTexture = 0;
        int mTexWidth = 0;
        int mTexHeight = 0;
        int mNumberOfChannels = 0;
        std::string mTextureName;
    };
}

#endif  // ANIMAFLOW_TEXTURE_HPP
