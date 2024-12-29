#pragma once
#include "EventHeader.h"

/*
ParallelDeadLine Class; inherits Event Class

Runs multiple Events at the sametime, but runs one of the as a deadline
When the deadline Event ends, all the Events still running will cancel
*/
struct ParallelDeadline : Event {
public:
	ParallelDeadline();
	ParallelDeadline(Event* deadline, std::initializer_list<Event*> events);
protected:
	void addEvents(std::initializer_list<Event*> events);
	void addDeadline(Event* deadline);

private:
	std::vector<Event*> events;
	Event* deadline;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};