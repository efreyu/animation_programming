#pragma once
#ifndef ANIMAFLOW_OGL_RENDERER_HPP
#define ANIMAFLOW_OGL_RENDERER_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "framebuffer.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"
#include "shader.hpp"

#include "ogl_render_data.hpp"

namespace af:: inline platform::opengl {
    class OGLRenderer {
    public:
        bool init(unsigned int width, unsigned int height);
        void setSize(unsigned int width, unsigned int height);
        void uploadData(OGLMesh vertexData);
        void draw();
        void cleanup();

    private:
        Shader mShader{};
        Framebuffer mFramebuffer{};
        Texture mTex{};
        VertexBuffer mVertexBuffer{};

        int mTriangleCount = 0;
    };
}  // namespace af::inline platform::opengl

#endif  // ANIMAFLOW_OGL_RENDERER_HPP
