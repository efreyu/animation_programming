#include "texture.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "core/assert.hpp"
#include "stb_image.h"

using namespace af::opengl;

void Texture::cleanup() {
    glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(const std::string& textureFilename) {
    mTextureName = textureFilename;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *textureData = stbi_load(textureFilename.c_str(), &mTexWidth, &mTexHeight, &mNumberOfChannels, 0);

    if (!textureData) {
        error_msg_format("Could not load file '{}'", mTextureName);
        stbi_image_free(textureData);
        return false;
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexWidth, mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(textureData);

    info_msg_format("Texture '{}' loaded ({}x{}, {} channels)", mTextureName, mTexWidth, mTexHeight, mNumberOfChannels);
    return true;
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}