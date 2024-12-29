#pragma once
#include "EventHeader.h"

/*
SequentialEvent Class; inherits Event Class

Group of events that are scheduled one after the other in a sequence
Avoid using non self-terminating Events
*/
class SequentialEvent : Event {
public:
	//Default constructor for inheritance
	SequentialEvent();

	/**
	* Constructor
	* 
	* @param List of events to run in sequence
	*/
	SequentialEvent(std::initializer_list<Event*> events);

	/**
	* Wrapper Constructor for Event Class
	* 
	* @param Event to run first
	* @param List of events to run afterwards in sequence
	*/
	SequentialEvent(Event* first, std::initializer_list<Event*> following);
protected:

	/**
	* Adds Events in a list to run
	* 
	* @param Initializer list of events to add
	*/
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