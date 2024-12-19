#include "../../../include/TaffyEngine.h"

/*
WaitUntil Event

Runs until condition is met or timer is up
Used for a condition or timer event for other events, especially event groups


*/

WaitUntil::WaitUntil(long ms) {
	time = ms;
}

WaitUntil::WaitUntil(bool (*condition)()) {
	time = -1;
	this->condition = condition;
}

void WaitUntil::init() { start = GetTickCount(); }
void WaitUntil::loop() {}
void WaitUntil::end(bool interrupted) {}
bool WaitUntil::isFinished() {
	if (condition != nullptr) {
		return !condition();
	}
	else {
		return start + time < GetTickCount();
	}
}