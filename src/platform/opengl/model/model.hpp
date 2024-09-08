#pragma once
#ifndef ANIMAFLOW_MODEL_HPP
#define ANIMAFLOW_MODEL_HPP

#include "platform/opengl/render/ogl_render_data.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace af::inline platform::opengl {
    class Model {
    public:
        void init();

        OGLMesh getVertexData();

    private:
        OGLMesh mVertexData;
    };
}  // namespace af

#endif  // ANIMAFLOW_MODEL_HPP
