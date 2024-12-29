#include "../../include/base/EngineBase.h"

std::function<void()> init_outside = []() {};
std::function<void()> loop_outside = []() {};

void EngineBase::initialize() {
    Win32Render::updateValues();

    init_outside();
    Controls::initialize();
}
void EngineBase::execute() {
    Win32Render::clearScreen(0x000000);

    loop_outside();

    Controls::update();
    ObjectHandler::updateAllObjects();
	EventHandler::runEvents();
    SpriteHandler::handleSprites();
	ObjectHandler::renderAllObjects();

    Win32Render::draw_number(Win32Window::delta_time * 10000, 0, 0, 50);
}
void EngineBase::run(std::function<void()> init, std::function<void()> loop) {
    init_outside = init;
    loop_outside = loop;
    Win32Window::run(initialize, execute);
}