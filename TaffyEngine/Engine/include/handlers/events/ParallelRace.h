#pragma once
#include "EventHeader.h"

/*
ParallelRace Class; inherits Event Class

Runs multiple Events at the same time
When the first Event ends, the rest of the events will cancel
*/
class ParallelRace : Event {
public:

	//Default constructor for inheritance
	ParallelRace();

	/**
	* Constructor
	* 
	* @param Events to add that will run parallel
	*/
	ParallelRace(std::initializer_list<Event*> events);

	/**
	* Wrapper Constructor for Event Class
	*
	* @param Event to add that will run parallel
	* @param Other Events to add that will run parallel
	*/
	ParallelRace(Event* event, std::initializer_list<Event*> events);
	
protected:

	/**
	* Add events to a list that will run them parallel
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