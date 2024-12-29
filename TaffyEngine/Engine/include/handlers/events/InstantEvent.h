#pragma once
#include "EventHeader.h"

/*
InsantEvent Class; inherits Event Class

Event that runs a Lambda method instantly
Will run the method on initialize, then self-terminate
*/
class InstantEvent final : Event {

public:
	/**
	* Constructs the InstantEvent
	* 
	* @param Lambda method to run
	* @param Required Objects
	*/
	InstantEvent(std::function<void()> instant, std::initializer_list<Object*> requirments);
private:
	std::function<void()> instant = []() {};

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};