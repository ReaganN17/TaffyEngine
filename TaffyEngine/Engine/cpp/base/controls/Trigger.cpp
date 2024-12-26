#include "../../../include/base/controls/Trigger.h"
#include "../../../include/base/Input.h"

void Trigger::update() {
	switch (type) {
	case ON_TRUE: {
		if (Input::is_pressed(button)) {
			allocateEvent();
		}
	} break;
	case ON_FALSE: {
		if (Input::is_released(button)) {
			allocateEvent();
		}
	} break;
	case ON_CHANGE: {
		if (Input::is_pressed(button) || Input::is_released(button)) {
			allocateEvent();
		}
	} break;

	case WHILE_FALSE: {
		if (Input::is_pressed(button)) {
			deallocateEvent();
		}
		if (Input::is_released(button)) {
			allocateEvent();
		}
	} break;
	case WHILE_TRUE: {
		if (Input::is_pressed(button)) {
			allocateEvent();
		}
		if (Input::is_released(button)) {
			deallocateEvent();
		}
	} break;

	case TOGGLE_ON_FALSE: {
		if (Input::is_pressed(button) && event != nullptr) {
			allocateEvent();
		}
		if (Input::is_pressed(button) && event == nullptr) {
			deallocateEvent();
		}
	} break;

	case TOGGLE_ON_TRUE: {
		if (Input::is_pressed(button) && event == nullptr) {
			allocateEvent();
		}
		if (Input::is_pressed(button) && event != nullptr) {
			deallocateEvent();
		}
	} break;
	}
}

void Trigger::initialize() {
	switch (type) {
	case WHILE_FALSE:
	case TOGGLE_ON_FALSE:{
		allocateEvent();
	} break;
	}
}

void Trigger::allocateEvent() {
	deallocateEvent();
	event = factory();
	event->schedule();
}
void Trigger::deallocateEvent() {
	if (event != nullptr) event->cancel();
	event = nullptr;
}

Trigger::Trigger(u8 button, TriggerType type, std::function<Event* ()> factory) 
:button(button), type(type), factory(factory){}

Trigger::~Trigger() {
	deallocateEvent();
}
