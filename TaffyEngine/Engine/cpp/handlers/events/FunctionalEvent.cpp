#include "../../../include/TaffyEngine.h"
/*
Functional Event

Event made by lambdas
*/

FunctionalEvent::FunctionalEvent(std::function<void()> init_lamb, std::function<void()> loop_lamb, std::function<void()> end_lamb, std::function<bool()> bool_lamb, std::initializer_list<Object*> requirements) {
	this->init_lamb = init_lamb;
	this->loop_lamb = loop_lamb;
	this->end_lamb = end_lamb;
	this->bool_lamb = bool_lamb;
	addRequirements(requirements);
}
void FunctionalEvent::init() {
	init_lamb();
}

void FunctionalEvent::loop() {
	loop_lamb();
}
void FunctionalEvent::end(bool interrupted) {
	end_lamb();
}
bool FunctionalEvent::isFinished() {
	return bool_lamb();
}