#pragma once

#include "../utils/Utils.h"
#include "objects/Object.h"
#include "events/Event.h"

/*
Event Handler Class

Contains, manages and runs Events;
*/
class EventHandler final {
	friend class Event;
	friend class EngineBase;

private:
	/**
	* Adds an Event to the Event list
	* 
	* @param Event pointer to add
	* @return true if successful, false if the Event is already present
	*/
	static bool addEvent(Event* event);

	/**
	* Removes an Event from the Event list
	* 
	* @param Event pointer to remove
	* @returns true if successful, false if the Event was not present
	*/
	static bool removeEvent(Event* event);
	
	static std::vector<Event*> events;

	//Disable Construction
	EventHandler() = delete;

	//Runs the scheduled Events
	static void runEvents();
public:
	/**
	* Schedules the Event to start running
	* 
	* @param Event to schedule
	*/
	static void scheduleEvent(Event* event);

	/**
	* Checks if the requirement Objects match any of the given Objects in every scheduled Event
	*
	* @param Initializer List of Objects to check
	* @return If any requirement Objects match
	*/
	static bool checkRequirements(std::initializer_list<Object*> objects);

	/**
	* Checks if the requirement Objects match any of the given Objects in every scheduled Event
	*
	* @param Vector of Objects to check
	* @return If any requirement Objects match
	*/
	static bool checkRequirements(std::vector<Object*> objects);

	/**
	* Cancels any scheduled Event that contain matching requirement Objects
	*
	* @param Initializer List of Objects to check
	*/
	static void cancelRequirements(std::initializer_list<Object*> objects);

	/**
	* Cancels any scheduled Event that contain matching requirement Objects
	* 
	* @param Vector List of Objects to check
	*/
	static void cancelRequirements(std::vector<Object*> objects);

	//Cancels all scheduled Events and clears the list
	static void clearEvents();
};