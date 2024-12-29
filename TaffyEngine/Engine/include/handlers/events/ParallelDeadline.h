#pragma once
#include "EventHeader.h"

/*
ParallelDeadLine Class; inherits Event Class

Runs multiple Events at the same time, but runs one of the as a deadline
When the deadline Event ends, all the Events still running will cancel
*/
struct ParallelDeadline : Event {
public:
	//Default Constructor for inheritance
	ParallelDeadline();

	/**
	* Constructs the Event
	* 
	* @param Deadline Event
	* @param Other Events that will run in parallel
	*/
	ParallelDeadline(Event* deadline, std::initializer_list<Event*> events);
protected:

	/**
	* Adds Parallel Events
	* 
	* @param Events that will run in parallel
	*/
	void addEvents(std::initializer_list<Event*> events);

	/**
	* Sets the deadline Event
	* 
	* @param Event that will be the deadline 
	*/
	void setDeadline(Event* deadline);

private:
	std::vector<Event*> events;
	Event* deadline;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};