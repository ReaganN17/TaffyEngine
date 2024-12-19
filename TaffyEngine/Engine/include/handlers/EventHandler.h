#pragma once

#include "events/EventHeader.h"
#include "objects/Objects.h"

struct EventHandler {
private:
	
	static EventHandler instance;
public:
	std::vector<Event*> events;

	static EventHandler& getInstance();

	void scheduleEvent(Event* event);

	bool addEvent(Event* event);
	void runEvents();
	void clearEvents();
	bool removeEvent(Event* event);

	bool checkRequirements(std::vector<Object*> objects);

	void cancelRequirements(std::vector<Object*> objects);
};