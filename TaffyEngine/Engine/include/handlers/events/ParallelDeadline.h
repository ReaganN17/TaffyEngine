#pragma once
#include "EventHeader.h"
struct ParallelDeadline : Event {
public:
	ParallelDeadline();
	ParallelDeadline(Event* deadline, std::initializer_list<Event*> events);
protected:
	void addEvents(std::initializer_list<Event*> events);

	
private:
	std::vector<Event*> events;
	Event* deadline;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};