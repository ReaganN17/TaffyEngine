#pragma once
#include "EventHeader.h"
struct  ParallelRace : Event {
public:
	ParallelRace();
	ParallelRace(std::initializer_list<Event*> events);
	ParallelRace(Event* event, std::initializer_list<Event*> events);
	
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