#include "../../../include/TaffyEngine.h"

/*
RepeatEvent

Frame for the repeating event
when it ends, it inits and starts running again

ends when canceled
*/

RepeatEvent::RepeatEvent(Event* event) {
	this->event = event;

	addRequirements(event->getRequirements());
}

void RepeatEvent::init() {
	event->init();
	event->setRunning(true);
}
void RepeatEvent::loop() {
	event->loop();

	if (event->isFinished()) {
		event->end(false);
		event->init();
	}
}
void RepeatEvent::end(bool interrupted) {
	event->end(true);
}
bool RepeatEvent::isFinished() {
	return false;
}

void RepeatEvent::destruct() {
	event->destruct();

	Event::destruct();
}