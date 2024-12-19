#pragma once

#include "EventHeader.h"

struct ConditionalEvent final : Event {
public:
	ConditionalEvent(Event* onTrue, Event* onFalse, bool(*condition)());
protected:
private:
	Event* on_true = nullptr;
	Event* on_false = nullptr;
	bool(*condition)() = nullptr;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};