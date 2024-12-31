#pragma once

#include "Utils.h"
/*
Timer Struct

Simple Timer; units in milliseconds
*/
struct Timer final {
private:
	long start_time = 0;
public:

	//Default Constructor
	Timer();

	//Starts the timer
	void start();

	//Gets time since started
	long getTime();

	//@return If time has passed the given time
	bool hasElapsed(long time);
};