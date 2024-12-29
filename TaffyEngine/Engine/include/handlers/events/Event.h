#pragma once

#include "EventHeader.h"
#include "../EventHandler.h"

/*
Event Class

Events hold 4 required methods: init, loop, end, and isFinished
User can create Event, by class inheritance, to wrap methods to be called in more advanced ways,
*/
class Event {
	friend ParallelRace;
	friend ParallelEvent;
	friend ParallelDeadline;
	friend SequentialEvent;
	friend RepeatEvent;
	friend ConditionalEvent;
	friend EventHandler;

public:
	//Default Constructor
	Event();
	
	//Schedules the event; Initializes and runs on loop
	virtual void schedule();

	//Cancels the event; will interrupt the event regardless of its interruptability
	virtual void cancel();

	/**
	* Returns if the event is currently running
	* 
	* @return If the event is currently running
	*/
	bool isRunning();

	/**
	* Returns if the Event is linked to an oustide variable; meaning there is a variable somewhere holding its value
	* Prevents internal destruction, leaving behind a bad variable
	* 
	* @return If the Event is linked to an outside variable
	*/
	bool isLinked();

	/**
	* Sets it linkage if its linked to an outside variable
	* 
	* @param set or not set if its linked
	*/
	void setLink(bool link);

	/**
	* Allows this Event to be ran with other Events
	* Creates a ParallelEvent containing this Event and others given
	* 
	* @param List of Events to schedule with it
	* @return ParallelEvent*
	*/
	ParallelEvent* alongWith(std::initializer_list<Event*> events);

	/**
	* Allows an Event or Events to be scheduled after this Event ends
	* Creates a SequentialEvent containing this Event and others given
	* 
	* @param List of Events to be scheduled in sequence
	* @return SequentialEvent*
	*/
	SequentialEvent* andThen(std::initializer_list<Event*> events);

	/**
	* Allows a lambda method to be called once after this event ends
	* Creates a SequtialEvent containg this Event and an InstantEvent containing the method
	* 
	* @param Lambda method to be ran afterwards
	* @param Object Requirments
	* @return SequentialEvent*
	*/
	SequentialEvent* andThen(std::function<void()> run, std::initializer_list<Object*> requirements);

	/**
	* Allows an Event to be ran before running this Event
	* Creates a SequentialEvent containg this Event and the other given
	* 
	* @param Event to ran beforehand
	* @return SequentialEvent*
	*/
	SequentialEvent* beforeStarting(Event* before);

	/**
	* Allows a lambda method to be ran before running this Event
	* Creates a SequentialEvent containg this Event and the lambda given
	*
	* @param Lambda method to ran beforehand
	* @param Object Requirements
	* @return SequentialEvent*
	*/
	SequentialEvent* beforeStarting(std::function<void()> before, std::initializer_list<Object*> requirements);

	/**
	* Allows this Event to run with other Events, and will all end when this Event ends;
	* Creates a ParallelDeadline with this Event as deadline and containing the others given
	* 
	* @param List of Events to schedule with it
	* @return ParallelDeadline*
	*/
	ParallelDeadline* deadlineWith(std::initializer_list<Event*> events);

	/**
	* Allows this Event to only run if a condition is true
	* Creates a ConditionalEvent containing this Event, an empty Event, and the condition method given
	* 
	* @param Lambda method condition to check
	* @return ConditionalEvent*
	*/
	ConditionalEvent* onlyIf(std::function<bool()> condition);

	/**
	* Allows this Event to only run while a condition is true
	* Creates a ParallelRace containing this Event and a WaitUntil containing the condition
	* 
	* @param Labmda method to check
	* @return ParallelRace*
	*/
	ParallelRace* onlyWhile(std::function<bool()> condition);

	/**
	* Allows this Event to run with other Events, and will all end when the first Event ends
	* Creates a ParallelRace containing this Event and others given
	* 
	* @param List of Events to run
	* @return ParallelRace*
	*/
	ParallelRace* raceWith(std::initializer_list<Event*> parallel);

	/**
	* Allows this Event to run on repeat, once it ends, it initializes again;
	* Can end via cancel()
	* Creates a RepeatEvent wrapping this Event
	* 
	* @return RepeateEvent*
	*/
	RepeatEvent* repeatedly();

	/**
	* Allows this Event to run unless a condition is true
	* Creates a ConditionalEvent containing an empty Event, this Event, and the condition method given
	* 
	* @param Lambda method to check
	* @return ConditionalEvent*
	*/	
	ConditionalEvent* unless(std::function<bool()> condition);

	/**
	* Allows this Event to run until a condition is true
	* Creates a ParallelRace containing this Event and a WaitUntil containing the condition
	* 
	* @param Labmda method to check
	* @return ParallelRace*
	*/
	ParallelRace* until(std::function<bool()> condition);

	/**
	* Allows this Event to end based on time or when it ends itself before the time ends
	* Creates a ParallelRace containing this Event and a WaitUntil containing the time
	* 
	* @param Time in milliseconds
	* @return ParallelRace*
	*/
	ParallelRace* withTimeout(long ms);

	/**
	* Returns list of Objects required in the event
	* 
	* @return reference of the vector of Object pointers
	*/
	std::vector<Object*>& getRequirements();

protected:
	/**
	* Adds required objects to an Event
	* Required objects are objects that must only be used or not used in certain events at the same time
	* For example, an Event that moves an object can conflict with another event that moves the same object;
	* required objects allow one of the events to be cancelled due to the existence of the other event
	* 
	* @param Objects to be added to required Objects list
	*/
	virtual void addRequirements(std::initializer_list<Object*> objs);

	/**
	* Adds required objects to an Event
	* Required objects are objects that must only be used or not used in certain events at the same time
	* For example, an Event that moves an object can conflict with another event that moves the same object;
	* required objects allow one of the events to be cancelled due to the existence of the other event
	*
	* @param Vector of objects to be added to required Objects list
	*/
	virtual void addRequirements(std::vector<Object*> objs);

	/**
	* Adds required objects to an Event
	* Required objects are objects that must only be used or not used in certain events at the same time
	* For example, an Event that moves an object can conflict with another event that moves the same object;
	* required objects allow one of the events to be cancelled due to the existence of the other event
	*
	* @param Object to be added to required Objects list
	*/
	virtual void addRequirements(Object* object);
	
	//Initialization of the Event; runs once
	//Abstract method, required to use on class inhertance
	virtual void init() = 0;

	//Loop of the Event; runs on loop
	//Abstract method, required to use on class inhertance
	virtual void loop() = 0;

	/**
	* End part of the Event; runs once
	* Abstract method, required to use on class inhertance
	* 
	* @param If the event was interrupted; ie self-termination or cancelled externally
	*/
	virtual void end(bool interrupted) = 0;

	/**
	* Condition to check if the Event has finished
	* Allows the event to self terminate if needed
	* Abstract method, required to use on class inhertance
	* 
	* @return If the Event has finished
	*/
	virtual bool isFinished() = 0;
	
private:
	bool running = false;
	bool linked = false;

	std::vector<Object*> requirements;
	
	//Ends the event without interruption
	//Used only by self-termination
	virtual void kill();

	//Run method that does all the internal work of the Event
	//EventHandler uses this method to run Events
	void run();

	//Destroys the Event
	virtual void destruct();

	//Sets the running status of the Event
	void setRunning(bool run);
}; 