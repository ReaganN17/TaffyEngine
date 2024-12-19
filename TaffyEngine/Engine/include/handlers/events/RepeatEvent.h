#pragma once
#include "EventHeader.h"
struct RepeatEvent final : Event {
public:
	RepeatEvent(Event* event);
protected:
private:
	Event* event;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};