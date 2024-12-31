#pragma once

#include "../../utils/Utils.h"
#include "../../handlers/events/EventHeader.h"

//Trigger Types
enum TriggerType : u8 {
	ON_CHANGE, //Schedules Event on condition change
	ON_TRUE, //Schedules Event on condition change to true
	ON_FALSE, //Schedules Event on condition change to false
	WHILE_TRUE, //Schedules Event on condition change to true; cancels on condition change to false
	WHILE_FALSE, //Schedules Event on condition change to false; cancels on condition change to true
	TOGGLE_ON_TRUE, //Schedules or cancels Event on condition change to true
	TOGGLE_ON_FALSE //Schedules or cancels Event on condition change to false
};

/*
Trigger Class

Allows an event to be scheduled based on condition activity
*/
class Trigger final {
	friend class Controls;

	std::function<Event* ()> factory = []() {return (Event*) new WaitUntil((long)0); };
	std::function<bool()> condition = []() {return false; };
	Event* event = nullptr;
	TriggerType type = ON_CHANGE;
	bool previous_condition = false;

	//Updates the trigger periodically
	void update();

	//Initializes the trigger
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
	* @param Condition to be associated with
	* @param Trigger type
	* @param Method to construct the event
	*/
	Trigger(std::function<bool()> condition, TriggerType type, std::function<Event* ()> factory);
};