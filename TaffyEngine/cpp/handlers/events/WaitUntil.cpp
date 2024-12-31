#include "../../../include/handlers/events/WaitUntil.h"

WaitUntil::WaitUntil(long ms) {
	time = ms;
}

WaitUntil::WaitUntil(std::function<bool()> condition) {
	time = -1;
	this->condition = condition;
}

void WaitUntil::init() { 
	timer.start();
}
void WaitUntil::loop() {}
void WaitUntil::end(bool interrupted) {}
bool WaitUntil::isFinished() {
	if (time < 0) {
		return condition();
	}
	else {
		return timer.hasElapsed(time);
	}
}