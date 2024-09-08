#pragma once
#ifndef ANIMAFLOW_OGLRENDERDATA_HPP
#define ANIMAFLOW_OGLRENDERDATA_HPP

#include <glm/glm.hpp>
#include <vector>

namespace af::inline platform::opengl {

    struct OGLVertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    struct OGLMesh {
        std::vector<OGLVertex> vertices;
    };
}  // namespace af

#endif  // ANIMAFLOW_OGLRENDERDATA_HPP
