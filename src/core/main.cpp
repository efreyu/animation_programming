#include <memory>

#include "core/window.hpp"
#include "core/assert.hpp"

using namespace af;

int main(int argc, char *argv[]) {
    std::unique_ptr<core::Window> w = std::make_unique<core::Window>();

    if (!w->init(640, 480, "AnimaFlow: Game Animations Programming")) {
        error_msg("Window init error");
        return -1;
    }

    w->mainLoop();

    w->cleanup();

    return 0;
}