#include "../../../include/handlers/events/InstantEvent.h"

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

