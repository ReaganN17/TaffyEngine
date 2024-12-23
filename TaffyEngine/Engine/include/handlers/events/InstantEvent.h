#pragma once
#include "EventHeader.h"
struct InstantEvent final : Event {
public:
	InstantEvent(std::function<void()> instant, std::initializer_list<Object*> requirments);
private:
	std::function<void()> instant = []() {};

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};