#pragma once

#include "EventHeader.h"


/*
ConditionalEvent Class; inherits Event Class

Given two events and a condition, will check condition upon initialization and select one of the events based on the condition
*/
struct ConditionalEvent final : Event {
public:

	/**
	* Constructs ConditionalEvent
	* 
	* @param Event to do if condition is true
	* @param Event to do if condition is false
	* @param Condition method to check, can be passed as lambda
	*/
	ConditionalEvent(Event* onTrue, Event* onFalse, std::function<bool()> condition);
private:

	Event* on_true = nullptr;
	Event* on_false = nullptr;
	std::function<bool()> condition = []() {return false; };

	//Checks condition upon initialization and initializes the corresponding Event
	void init();

	//Based on condition will loop the corresponding Event
	void loop();
	
	/**
	* Based on condition will end the corresponding Event
	* 
	* @param if the event was interrupted to end
	*/
	void end(bool interrupted);

	//Based on condition will check if the correspoding Event is Finished
	bool isFinished();

	//Destructs the two events and itself
	void destruct();
};