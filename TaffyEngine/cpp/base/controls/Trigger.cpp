#include "../../../include/base/controls/Trigger.h"

void Trigger::update() {
	switch (type) {
	case ON_TRUE: {
		if (!previous_condition && condition()) {
			allocateEvent();
		}
	} break;
	case ON_FALSE: {
		if (previous_condition && !condition()) {
			allocateEvent();
		}
	} break;
	case ON_CHANGE: {
		if (previous_condition != condition()) {
			allocateEvent();
		}
	} break;

	case WHILE_FALSE: {
		if (!previous_condition && condition()) {
			deallocateEvent();
		}
		if (previous_condition && !condition()) {
			allocateEvent();
		}
	} break;
	case WHILE_TRUE: {
		if (!previous_condition && condition()) {
			allocateEvent();
		}
		if (previous_condition && !condition()) {
			deallocateEvent();
		}
	} break;

	case TOGGLE_ON_FALSE: {
		if (!previous_condition && condition() && event != nullptr) {
			allocateEvent();
		}
		if (!previous_condition && condition() && event == nullptr) {
			deallocateEvent();
		}
	} break;

	case TOGGLE_ON_TRUE: {
		if (!previous_condition && condition() && event == nullptr) {
			allocateEvent();
		}
		if (!previous_condition && condition() && event != nullptr) {
			deallocateEvent();
		}
	} break;
	}

	previous_condition = condition();

	if (event != nullptr && !event->isRunning()) { 
		event->setLink(false); 
		event->destruct(); 
		event = nullptr;
	}
}

void Trigger::initialize() {
	previous_condition = !condition();
}

void Trigger::allocateEvent() {
	deallocateEvent();
	event = factory();
	event->setLink(true);
	event->schedule();
}
void Trigger::deallocateEvent() {
	if (event != nullptr) event->cancel();
	event = nullptr;
}

Trigger::Trigger(std::function<bool()> condition, TriggerType type, std::function<Event* ()> factory)
:condition(condition), type(type), factory(factory){}

Trigger::~Trigger() {
	deallocateEvent();
}
