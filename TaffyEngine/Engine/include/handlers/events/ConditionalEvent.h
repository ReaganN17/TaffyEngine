#pragma once

#include "EventHeader.h"

struct ConditionalEvent final : Event {
public:
	ConditionalEvent(Event* onTrue, Event* onFalse, std::function<bool()> condition);
protected:
private:
	Event* on_true = nullptr;
	Event* on_false = nullptr;
	std::function<bool()> condition = []() {return false; };

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};