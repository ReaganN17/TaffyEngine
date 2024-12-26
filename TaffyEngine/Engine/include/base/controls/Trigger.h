#pragma once

#include "../../TaffyEngine.h"

class Controls;

//Trigger Types
enum TriggerType : u8 {
	ON_CHANGE, //Schedules Event on button change
	ON_TRUE, //Schedules Event on button press
	ON_FALSE, //Schedules Event on button release
	WHILE_TRUE, //Schedules Event on button press; cancels on release
	WHILE_FALSE, //Schedules Event on button release; cancels on press
	TOGGLE_ON_TRUE, //Schedules or cancels Event on button press; starts on schedule
	TOGGLE_ON_FALSE //Schedules or cancels Event on button press; starts on cancel
};

/*
Trigger Class

Allows event to be scheduled based on button activity
*/
class Trigger final {
	friend Controls;

	std::function<Event* ()> factory = []() {return new WaitUntil((long)0); };
	Event* event = nullptr;
	u8 button = 0;
	TriggerType type = ON_CHANGE;

	//Updates the trigger periodically
	void update();

	//Initializes the trigger; specifacally WHILE_FALSE and TOGGLE_ON_FALSE to schedule an event beforehand
	void initialize();

	//Allocates and schedules the event via the event factory
	void allocateEvent();

	//Deallocates and cancels the event
	void deallocateEvent();

	//Deconstructor
	~Trigger();
public: 

	/**
	* Trigger Constructor
	* 
	* @param Button to be associated with
	* @param Trigger type
	* @param Method to construct the event
	*/
	Trigger(u8 button, TriggerType type, std::function<Event* ()> factory);
};