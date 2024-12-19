#pragma once
#include "EventHeader.h"
struct SequentialEvent : Event {
public:
	SequentialEvent();
	SequentialEvent(std::initializer_list<Event*> events);
	SequentialEvent(Event* first, std::initializer_list<Event*> following);
protected:
	void addEvents(std::initializer_list<Event*> events);
private:
	std::vector<Event*> events;
	u16 iterator = 0;
	
	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};