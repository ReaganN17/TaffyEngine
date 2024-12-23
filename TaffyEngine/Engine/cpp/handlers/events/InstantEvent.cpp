#include "../../../include/TaffyEngine.h"
/*
InstantEvent

method that runs on init then ends instantly
*/

InstantEvent::InstantEvent(std::function<void()> instant, std::initializer_list<Object*> requirements) {
	this->instant = instant;
	addRequirements(requirements);
}

void InstantEvent::init() {
	instant();
}

void InstantEvent::loop() {}
void InstantEvent::end(bool interrupted) { }
bool InstantEvent::isFinished() { return true; }

