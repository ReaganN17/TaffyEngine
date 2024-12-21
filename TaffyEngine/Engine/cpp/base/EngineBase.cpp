#include "../../include/TaffyEngine.h"

std::function<void()> EngineBase::init_outside = []() {};
std::function<void()> EngineBase::loop_outside = []() {};

void EngineBase::initialize() {
    Win32Render::updateValues();

    init_outside();
}
void EngineBase::execute() {
    Win32Render::clearScreen(0x000000);

    loop_outside();

    ObjectHandler::getInstance().updateAllObjects();
	EventHandler::getInstance().runEvents();
	ObjectHandler::getInstance().renderAllObjects();

    
}
void EngineBase::run(std::function<void()> init, std::function<void()> loop) {
    init_outside = init;
    loop_outside = loop;
    Win32Window::run(initialize, execute);
}