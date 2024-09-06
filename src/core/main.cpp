#include <memory>

#include "core/window.hpp"

using namespace af;

int main(int argc, char *argv[]) {
    std::unique_ptr<core::Window> w = std::make_unique<core::Window>();

    if (!w->init(640, 480, "Test Window")) {
//        Logger::log(1, "%s error: Window init error\n", __FUNCTION__);
        return -1;
    }

    w->mainLoop();

    w->cleanup();

    return 0;
}