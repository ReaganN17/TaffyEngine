#pragma once
#include "EventHeader.h"
struct WaitUntil final : Event   {
public:
	WaitUntil(long ms);
	WaitUntil(std::function<bool()> condition);

private:
	long start = 0;
	long time = 0;
	std::function<bool()> condition = []() {return false; };
	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};