#include "shader.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cerrno>  // errno
#include <cstring> // strerror()
#include "core/assert.hpp"

using namespace af::opengl;

bool Shader::loadShaders(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    info_msg_format("Loading vertex shader '{}' and fragment shader '{}'", vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

    if (!createShaderProgram(vertexShaderFileName, fragmentShaderFileName)) {
        error_msg("Shader program creation failed");
        return false;
    }

    return true;
}

void Shader::cleanup() {
    glDeleteProgram(mShaderProgram);
}

void Shader::use() {
    glUseProgram(mShaderProgram);
}

GLuint Shader::loadShader(std::string shaderFileName, GLuint shaderType) {
    std::string shaderAsText;
    shaderAsText = loadFileToString(shaderFileName);
    info_msg_format("loaded shader file '{}', size {}", shaderFileName, shaderAsText.size());

    const char* shaderSource = shaderAsText.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar**) &shaderSource, 0);
    glCompileShader(shader);

    if (!checkCompileStats(shaderFileName, shader)) {
        error_msg_format("error: compiling shader '{}' failed", shaderFileName);
        return 0;
    }

    info_msg_format("shader '{}' compiled", shaderFileName);
    return shader;
}

bool Shader::createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    GLuint vertexShader = loadShader(vertexShaderFileName, GL_VERTEX_SHADER);
    if (!vertexShader) {
        error_msg_format("loading of vertex shader '{}' failed", vertexShaderFileName);
        return false;
    }

    GLuint fragmentShader = loadShader(fragmentShaderFileName, GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        error_msg_format("loading of fragment shader '{}' failed", fragmentShaderFileName);
        return false;
    }

    mShaderProgram = glCreateProgram();

    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);

    glLinkProgram(mShaderProgram);

    if (!checkLinkStats(vertexShaderFileName, fragmentShaderFileName, mShaderProgram)) {
        error_msg_format("error: linking of shaders '{}' and '{}' failed", vertexShaderFileName, fragmentShaderFileName);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }

    /* it is safe to delete the shaders here */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    error_msg_format("shader program {} successfully compiled from vertex shader '{}' and fragment shader '{}'", mShaderProgram, vertexShaderFileName, fragmentShaderFileName);
    return true;
}

bool Shader::checkCompileStats(std::string shaderFileName, GLuint shader) {
    GLint isShaderCompiled;
    int logMessageLength;
    std::vector<char> shaderLog;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
    if (!isShaderCompiled) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logMessageLength);
        shaderLog = std::vector<char>(logMessageLength + 1);
        glGetShaderInfoLog(shader, logMessageLength, &logMessageLength, shaderLog.data());
        shaderLog.at(logMessageLength) = '\0';
        error_msg_format("shader compile of shader '{}' failed", shaderFileName);
        error_msg_format("shader compile log:\n{}", shaderLog.data());
        return false;
    }

    return true;
}

bool Shader::checkLinkStats(std::string vertexShaderFileName, std::string fragmentShaderFileName, GLuint shaderProgram) {
    GLint isProgramLinked;
    int logMessageLength;
    std::vector<char> programLog;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isProgramLinked);
    if (!isProgramLinked) {
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logMessageLength);
        programLog = std::vector<char>(logMessageLength + 1);
        glGetProgramInfoLog(shaderProgram, logMessageLength, &logMessageLength, programLog.data());
        programLog.at(logMessageLength) = '\0';
        error_msg_format("error: program linking of shaders '{}' and '{}' failed", vertexShaderFileName, fragmentShaderFileName);
        error_msg_format("compile log:\n{}", programLog.data());
        return false;
    }

    return true;
}

std::string Shader::loadFileToString(std::string fileName) {
    std::ifstream inFile(fileName);
    std::string str;

    if (inFile.is_open()) {
        str.clear();
        // allocate string data (no slower realloc)
        inFile.seekg(0, std::ios::end);
        str.reserve(inFile.tellg());
        inFile.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(inFile)),
                   std::istreambuf_iterator<char>());
        inFile.close();
    } else {
        error_msg_format("error: could not open file {}", fileName);
        error_msg_format("error: system says '{}'", strerror(errno));
        return {};
    }

    if (inFile.bad() || inFile.fail()) {
        error_msg_format("error: error while reading file '{}'", fileName);
        inFile.close();
        return {};
    }

    inFile.close();
    info_msg_format("file {} successfully read to string", fileName);
    return str;
}