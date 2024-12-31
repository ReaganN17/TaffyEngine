#include "../../../include/handlers/objects/ScreenButton.h"
#include "../../../include/handlers/ObjectHandler.h"

ScreenButton::ScreenButton() : Object() {}

ScreenButton::ScreenButton(Object object, ButtonType type) : Object(object) {
	ObjectHandler::addObject(this);
	this->type = type;
}

void ScreenButton::update() {
	bool previous = state.down;
	state.changed = false;

	if (!disabled) {
		switch (type) {
		case TOGGLE: {
			if (is_hovered() && Input::is_pressed(button)) {
				state.down = !state.down;
			}
		} break;
		case HELD: {
			if (is_hovered() && Input::is_down(button)) {
				state.down = true;
			}
			else {
				state.down = false;
			}
		} break;
		case PULSE: {
			if (is_hovered() && Input::is_pressed(button)) {
				state.down = true;
			}
			else {
				state.down = false;
			}

		} break;
		case HOVER: {
			state.down = is_hovered();
		} break;
		}

		state.changed = previous != state.down;
	}
	
	periodic();
}

bool ScreenButton::is_down() { return state.down; }
bool ScreenButton::is_pressed() { return state.down && state.changed; }
bool ScreenButton::is_released() { return !state.down && state.changed; }
bool ScreenButton::is_hovered() {
	return
		Input::getMouse().x > getX() - getW() / 2 &&
		Input::getMouse().x < getX() + getW() / 2 &&
		Input::getMouse().y > getY() - getH() / 2 &&
		Input::getMouse().y < getY() + getH() / 2;
}

void ScreenButton::reset() { state.down = false; }
void ScreenButton::disable() { disabled = true; }
void ScreenButton::enable() { disabled = false; }

void ScreenButton::periodic() {}