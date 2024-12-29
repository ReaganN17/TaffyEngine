#pragma once
#include "EventHeader.h"

/*
RepeatEvent Class; inherits Event Class

Given an Event, will repeat it when it ends
Will only end with external call of cancel()
*/
class RepeatEvent final : Event {
public:
	/**
	* Constructor
	* 
	* @param Event to run on repeate
	*/
	RepeatEvent(Event* event);
protected:
private:
	Event* event = nullptr;

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();

	void destruct();
};