#include "../../../include/TaffyEngine.h"
/*
Functional Event

Event made by lambdas
*/

FunctionalEvent::FunctionalEvent(void (*init)(), void (*loop)(), void (*end)(), bool(*isFinshed)(), std::initializer_list<Object*> requirements) {
	this->init_lamb = init;
	this->loop_lamb = loop;
	this->end_lamb = end;
	this->isFinished_lamb = isFinshed;
	addRequirements(requirements);
}
void FunctionalEvent::init() {
	if (init_lamb != nullptr) init_lamb();
}

void FunctionalEvent::loop() {
	if (loop_lamb != nullptr) loop_lamb();
}
void FunctionalEvent::end(bool interrupted) {
	if (end_lamb != nullptr) this->end_lamb();
}
bool FunctionalEvent::isFinished() {
	if (isFinished_lamb != nullptr) this->isFinished_lamb();

	return false;
}