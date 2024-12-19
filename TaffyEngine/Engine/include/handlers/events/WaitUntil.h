#pragma once
#include "EventHeader.h"
struct WaitUntil final : Event   {
public:
	WaitUntil(long ms);
	WaitUntil(bool (*condition)());

private:
	long start = 0;
	long time = 0;
	bool (*condition)();
	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};