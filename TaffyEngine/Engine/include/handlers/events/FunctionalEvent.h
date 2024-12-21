#pragma once
#include "EventHeader.h"
struct FunctionalEvent : Event {
public:
	FunctionalEvent(std::function<void()> init_lamb, std::function<void()> loop_lamb, std::function<void()> end_lamb, std::function<bool()> bool_lamb, std::initializer_list<Object*> requirements);
private:
	std::function<void()> init_lamb = []() {};
	std::function<void()> loop_lamb = []() {};
	std::function<void()> end_lamb = []() {};
	std::function<bool()> bool_lamb = []() {return false; };

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
protected:
	
};