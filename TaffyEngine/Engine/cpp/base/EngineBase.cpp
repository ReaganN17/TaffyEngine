#include "../../include/TaffyEngine.h"

std::function<void()> EngineBase::init_outside = []() {};
std::function<void()> EngineBase::loop_outside = []() {};

void EngineBase::initialize() {
    Win32Render::updateValues();

    init_outside();
    Controls::getInstance().initialize();
}
void EngineBase::execute() {
    Win32Render::clearScreen(0x000000);

    loop_outside();

    Controls::getInstance().update();
    ObjectHandler::getInstance().updateAllObjects();
	EventHandler::getInstance().runEvents();
    SpriteHandler::getInstance().handleSprites();
	ObjectHandler::getInstance().renderAllObjects();
    Win32Render::draw_number(Win32Window::delta_time * 10000, 0, 0, 50);
}
void EngineBase::run(std::function<void()> init, std::function<void()> loop) {
    init_outside = init;
    loop_outside = loop;
    Win32Window::run(initialize, execute);
}