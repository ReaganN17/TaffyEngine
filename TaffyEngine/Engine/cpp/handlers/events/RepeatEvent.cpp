#include "../../../include/handlers/events/RepeatEvent.h"

RepeatEvent::RepeatEvent(Event* event) {
	this->event = event;

	addRequirements(event->getRequirements());
}

void RepeatEvent::init() {
	if (event == nullptr) event = new WaitUntil((long)0);

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