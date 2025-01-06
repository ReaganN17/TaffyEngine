#include "../../include/base/EngineBase.h"

std::function<void()> EngineBase::init_outside = []() {};
std::function<void()> EngineBase::loop_outside = []() {};

void EngineBase::initialize() {
    Win32Render::updateValues();

    init_outside();
    Controls::initialize();
}
void EngineBase::execute() {
    Win32Render::clearScreen(0x000000);

    loop_outside();

    Controls::update();
    SpriteHandler::handleSprites(1000);
    EventHandler::runEvents();
    ObjectHandler::updateAllObjects();
	ObjectHandler::renderAllObjects();
}
void EngineBase::run(std::function<void()> init, std::function<void()> loop) {
    init_outside = init;
    loop_outside = loop;
    Win32Window::run(initialize, execute);
}

void EngineBase::end() {
    Win32Window::running = false;
}