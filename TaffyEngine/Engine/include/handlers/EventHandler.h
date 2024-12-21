#pragma once

#include "events/EventHeader.h"
#include "objects/Objects.h"
#include "../base/EngineBase.h"

struct EventHandler {

	friend Event;

private:
	
	static EventHandler instance;

	bool addEvent(Event* event);
	bool removeEvent(Event* event);
	bool checkRequirements(std::vector<Object*> objects);

	void cancelRequirements(std::vector<Object*> objects);
public:
	std::vector<Event*> events;

	static EventHandler& getInstance();

	void scheduleEvent(Event* event);

	
	void runEvents();
	void clearEvents();
};