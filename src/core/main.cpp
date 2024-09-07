#include <memory>

#include "platform/platform_manager.hpp"
#include "platform/window_interface.hpp"
#include "core/assert.hpp"

using namespace af;

int main(int argc, char *argv[]) {
    auto window = PlatformManager::getInstance().createWindow();

    if (!window->init(640, 480, "AnimaFlow: Game Animations Programming")) {
        error_msg("Window init error");
        return -1;
    }

    window->mainLoop();

    window->cleanup();

    return 0;
}