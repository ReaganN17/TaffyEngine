#include "../../../include/TaffyEngine.h"


/*
RunEvent

method that runs and continues to run
*/
RunEvent::RunEvent(std::function<void()> running, std::initializer_list<Object*> requirements){
	this->running = running;
	addRequirements(requirements);
}

void RunEvent::init() {}
void RunEvent::loop() { running(); }
void RunEvent::end(bool interrupted) {}
bool RunEvent::isFinished() { return false; }