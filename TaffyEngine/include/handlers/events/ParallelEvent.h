#pragma once

#include "../../utils/Utils.h"

#include "Event.h"

/*
ParallelEvent Class; inherits Event Class

Runs multiple Events at the same time
Will end when all events end
*/
class ParallelEvent : public Event {
public:
	//Default constructor for inheritance
	ParallelEvent();

	/**
	* Constructor
	* 
	* @param Initializer list of events to run
	*/
	ParallelEvent(std::initializer_list<Event*> events);

	/**
	* Wrapper Constructor for Event Class
	* 
	* @param Event to run
	* @param Initializer list of other events to run as well
	*/
	ParallelEvent(Event* event, std::initializer_list<Event*> other_events);
protected:

	/**
	* Adds the Events to a list to run
	* 
	* @param Initializer list of events to add
	*/
	void addEvents(std::initializer_list<Event*> events);
private:
	std::vector<Event*> events;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};