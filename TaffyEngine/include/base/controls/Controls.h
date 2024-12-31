#pragma once

#include "Trigger.h"
#include "../../utils/Utils.h"

/*
Controls Class
 
Stores and manages all Triggers
*/
class Controls final{
private:
	static std::vector<Trigger*> triggers;
	
	//Disables construction
	Controls() = delete;
public:

	//initializes triggers; specifically WHILE_FALSE and TOGGLE_ON_FALSE as they schedule the event at the start
	static void initialize();

	//updates triggers periodically; allows the triggers to schedule and cancel events based on button status.
	static void update();

	/**
	* Adds the trigger to the controller so it can be used
	* 
	* @param Trigger Constructor
	*/
	static void addTrigger(Trigger* trigger);

	/**
	* Removes the trigger to the controller
	*
	* @param Trigger Pointer
	*/
	static void removeTrigger(Trigger* trigger);

	//Clears all Triggers
	static void clearTriggers();
};