#pragma once

#include "../../TaffyEngine.h"

//Forward Declaration
class Trigger;


/*
Controls Class
 
Basically just stores all the triggers
Only one instance; no outside construction
 
TODO: refine addTrigger method
*/
class Controls final{
	static Controls instance;
	std::vector<Trigger*> triggers;

	//Disables outside construction
	Controls();
public:

	/**
	* Get the one instance of Controls
	* 
	* @return reference to the instance of Controls
	*/
	static Controls& getInstance();

	//initializes triggers; specifically WHILE_FALSE and TOGGLE_ON_FALSE as they schedule the event at the start
	void initialize();

	//updates triggers periodically; allows the triggers to schedule and cancel events based on button status.
	void update();

	/**
	* Adds the trigger to the controller so it can be used
	* 
	* @param Trigger Constructor
	*/
	void addTrigger(Trigger* trigger);

	/**
	* Destroys any triggers associated with a specific button
	* 
	* @param Button to clear
	*/
	void clearTrigger(u8 button);

	//Clears all controls
	void clearControls();
};