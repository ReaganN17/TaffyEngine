#pragma once

#include "../../utils/Utils.h"
#include "Event.h"

/*
RunEvent Class; inherits Event Class

Given a method, will run it on loop until event is cancelled externally
*/
class RunEvent final : public Event {
public:
	/**
	* Constructor
	* 
	* @param lambda method to run on loop
	* @param Object requirements
	*/
	RunEvent(std::function<void()> run, std::initializer_list<Object*> requirements);
private:
	std::function<void()> running = []() {};

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};
