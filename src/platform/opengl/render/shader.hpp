#pragma once
#ifndef ANIMAFLOW_SHADER_HPP
#define ANIMAFLOW_SHADER_HPP

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace af::inline platform::opengl {
    class Shader {
    public:
        bool loadShaders(std::string vertexShaderFileName, std::string fragmentShaderFileName);
        void use();
        void cleanup();

    private:
        GLuint mShaderProgram = 0;

        bool createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
        GLuint loadShader(std::string shaderFileName, GLuint shaderType);
        std::string loadFileToString(std::string filename);
        bool checkCompileStats(std::string shaderFileName, GLuint shader);
        bool checkLinkStats(std::string vertexShaderFileName, std::string fragmentShaderFileName, GLuint shaderProgram);
    };
}

#endif  // ANIMAFLOW_SHADER_HPP
