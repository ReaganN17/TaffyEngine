#pragma once
#include "EventHeader.h"
struct RunEvent final : Event {
	RunEvent(std::function<void()> run, std::initializer_list<Object*> requirements);

	std::function<void()> running = []() {};

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};
