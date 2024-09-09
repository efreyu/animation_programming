#include "ogl_renderer.hpp"
#include "core/assert.hpp"

using namespace af::platform::opengl;

bool OGLRenderer::init(unsigned int width, unsigned int height) {

    /* initalize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        error_msg("failed to initialize GLAD");
        return false;
    }

    if (!GLAD_GL_VERSION_4_6) {
        error_msg("failed to get at least OpenGL 4.6");
        return false;
    }

    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    info_msg_format("OpenGL {}.{} initialized", majorVersion, minorVersion);

    if (!mFramebuffer.init(width, height)) {
        error_msg("could not init Framebuffer");
        return false;
    }
    info_msg("framebuffer successfully initialized");

    if (!mTex.loadTexture("textures/crate.png")) {
        error_msg("texture loading failed");
        return false;
    }
    info_msg("texture successfully loaded");

    mVertexBuffer.init();
    info_msg("vertex buffer successfully created");

    if (!mShader.loadShaders("shader/basic.vert", "shader/basic.frag")) {
        error_msg("shader loading failed");
        return false;
    }
    info_msg("shaders successfully loaded");

    return true;
}

void OGLRenderer::setSize(unsigned int width, unsigned int height) {
    /* handle minimize */
    if (width == 0 || height == 0) {
        return;
  }

    mFramebuffer.resize(width, height);
    glViewport(0, 0, width, height);

    info_msg_format("resized window to {}x{}", width, height);
}

void OGLRenderer::uploadData(OGLMesh vertexData) {
    mTriangleCount = vertexData.vertices.size() / 3;
    mVertexBuffer.uploadData(vertexData);
}

void OGLRenderer::draw() {
    /* draw to framebuffer */
    mFramebuffer.bind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    mShader.use();
    mTex.bind();
    mVertexBuffer.bind();

    mVertexBuffer.draw(GL_TRIANGLES, 0, mTriangleCount * 3);
    mVertexBuffer.unbind();
    mTex.unbind();

    mFramebuffer.unbind();

    /* blit color buffer to screen */
    mFramebuffer.drawToScreen();
}

void OGLRenderer::cleanup() {
    mShader.cleanup();
    mTex.cleanup();
    mVertexBuffer.cleanup();
    mFramebuffer.cleanup();
}
