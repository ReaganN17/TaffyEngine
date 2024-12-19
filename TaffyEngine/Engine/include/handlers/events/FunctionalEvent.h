#pragma once
#include "EventHeader.h"
struct FunctionalEvent : Event {
public:
	FunctionalEvent(void (*init)(), void (*loop)(), void (*end)(), bool(*isFinshed)(), std::initializer_list<Object*> requirements);
private:
	void (*init_lamb)() = nullptr;
	void (*loop_lamb)() = nullptr;
	void (*end_lamb)() = nullptr;
	bool (*isFinished_lamb)() = nullptr;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
protected:
	
};