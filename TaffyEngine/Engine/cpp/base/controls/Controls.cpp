#include "../../../include/base/controls/Controls.h"

Controls Controls::instance;

Controls::Controls() {}

Controls& Controls::getInstance() { return instance; }

void Controls::initialize() {
	for (auto trigger : triggers) {
		trigger->initialize();
	}
}

void Controls::update() {
	for (auto trigger : triggers) {
		trigger->update();
	}
}
void Controls::addTrigger(Trigger* trigger) {
	triggers.push_back(trigger);
}
void Controls::clearTrigger(u8 button) {
	//do i kill myself
	for (int i = triggers.size() - 1; i >= 0; i--) {
		if (triggers[i]->button == button) { 
			triggers[i]->~Trigger(); 
			triggers.erase(triggers.begin() + i);
		}
	}
}
void Controls::clearControls() {
	for (auto trigger : triggers) {
		trigger->~Trigger();
	}

	triggers.clear();
}
