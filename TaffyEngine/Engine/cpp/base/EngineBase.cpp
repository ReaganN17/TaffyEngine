#include "../../include/TaffyEngine.h"

void (*EngineBase::init)() = nullptr;
void (*EngineBase::loop)() = nullptr;

void EngineBase::initialize() {
    Win32Render::updateValues();

    init();
}
void EngineBase::execute() {
    Win32Render::clearScreen(0x000000);

    loop();

    ObjectHandler::getInstance().updateAllObjects();
	EventHandler::getInstance().runEvents();
	ObjectHandler::getInstance().renderAllObjects();

    
}
void EngineBase::run(void (*init)(), void (*loop)()) {
    EngineBase::init = init;
    EngineBase::loop = loop;
    Win32Window::run(initialize, execute);
}