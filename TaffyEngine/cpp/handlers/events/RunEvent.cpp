#include "../../../include/handlers/events/RunEvent.h"

RunEvent::RunEvent(std::function<void()> running, std::initializer_list<Object*> requirements){
	this->running = running;
	addRequirements(requirements);
}

void RunEvent::init() {}
void RunEvent::loop() { running(); }
void RunEvent::end(bool interrupted) {}
bool RunEvent::isFinished() { return false; }