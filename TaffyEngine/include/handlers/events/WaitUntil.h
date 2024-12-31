#pragma once

#include "../../utils/Utils.h"
#include "Event.h"

/**
* WaitUntil Class; inherits Event Class
* 
* Event that waits until a condition is true or a certain time has pass before self-terminating
*/
class WaitUntil final : public Event   {
public:
	/**
	* Constructor for time Event
	* 
	* @param Duration in milliseconds
	*/
	WaitUntil(long ms);

	/**
	* Constructor for condition Event
	*
	* @param Condition method
	*/
	WaitUntil(std::function<bool()> condition);

private:
	Timer timer;
	long time = 0;
	std::function<bool()> condition = []() {return false; };

	void init();
	void loop();
	void end(bool interrupted);
	bool isFinished();
};