
#include "../../../include/handlers/events/ConditionalEvent.h"
/*
ConditionalEvent

this or that given a boolean method
*/
ConditionalEvent::ConditionalEvent(Event* onTrue, Event* onFalse, std::function<bool()> condition) {
	on_true = onTrue;
	on_false = onFalse;
	this->condition = condition;

	addRequirements(on_true->getRequirements());
	addRequirements(on_false->getRequirements());
}

void ConditionalEvent::init() {
	post_condition = condition();

	if (post_condition) {
		on_true->init();
		on_true->setRunning(true);
	}
	else {
		on_false->init();
	}
}

void ConditionalEvent::loop() {
	if (post_condition) {
		on_true->loop();
	}
	else {
		on_false->loop();
	}
}

void ConditionalEvent::end(bool interrupted) {
	if (post_condition) {
		on_true->end(interrupted);
	}
	else {
		on_false->end(interrupted);
	}
}
bool ConditionalEvent::isFinished() {
	if (post_condition) {
		return on_true->isFinished();
	}
	else {
		return on_false->isFinished();
	}
}

void ConditionalEvent::destruct() {
	on_true->destruct();
	on_false->destruct();

	Event::destruct();
}