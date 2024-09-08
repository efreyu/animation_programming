#pragma once
#ifndef ANIMAFLOW_VERTEX_BUFFER_HPP
#define ANIMAFLOW_VERTEX_BUFFER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ogl_render_data.hpp"

namespace af {
    class VertexBuffer {
    public:
        void init();
        void uploadData(OGLMesh vertexData);
        void bind();
        void unbind();
        void draw(GLuint mode, unsigned int start, unsigned int num);
        void cleanup();

    private:
        GLuint mVAO = 0;
        GLuint mVertexVBO = 0;
    };
}  // namespace af

#endif  // ANIMAFLOW_VERTEX_BUFFER_HPP
