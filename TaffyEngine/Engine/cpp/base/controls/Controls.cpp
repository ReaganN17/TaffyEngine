#include "../../../include/base/controls/Controls.h"

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
	auto key = std::find(triggers.begin(), triggers.end(), trigger);
	if (key != triggers.end()) { triggers.erase(key); }
	triggers.push_back(trigger);
}

void Controls::removeTrigger(Trigger* trigger) {
	auto key = std::find(triggers.begin(), triggers.end(), trigger);
	if (key != triggers.end()) { triggers.erase(key); }
}
void Controls::clearTriggers() {
	for (auto trigger : triggers) {
		trigger->~Trigger();
	}

	triggers.clear();
}
