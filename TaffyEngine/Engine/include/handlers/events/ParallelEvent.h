#pragma once
#include "EventHeader.h"
struct ParallelEvent : Event {
public:
	ParallelEvent();
	ParallelEvent(std::initializer_list<Event*> events);
	ParallelEvent(Event* event, std::initializer_list<Event*> other_events);
protected:
	void addEvents(std::initializer_list<Event*> events);
private:
	std::vector<Event*> events;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};