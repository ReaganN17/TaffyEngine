#include "../../include/handlers/EventHandler.h"

std::vector<Event*> EventHandler::events;

bool EventHandler::addEvent(Event* event) {
	auto key = std::find(events.begin(), events.end(), event);
	if (key == events.end()) {
		events.push_back(event);
		return true;
	}
	return false;
}
void EventHandler::runEvents() {
	for (auto event : events) {
		event->run();
	}
}
void EventHandler::clearEvents() {
	for (auto event : events) {
		event->cancel();
	}

	events.clear();
}

bool EventHandler::removeEvent(Event* event) {
	auto key = std::find(events.begin(), events.end(), event);
	if (key != events.end()) {
		events.erase(key);
		return true;
	}

	return false;
}

bool EventHandler::checkRequirements(std::initializer_list<Object*> objects) {
	for (auto event : events) {
		for (auto object : objects) {
			if (find(event->getRequirements().begin(), event->getRequirements().end(), object) != event->getRequirements().end()) {
				return true;
			}
		}
	}
	return false;
}

bool EventHandler::checkRequirements(std::vector<Object*> objects) {
	for (auto event : events) {
		for (auto object : objects) {
			if (find(event->getRequirements().begin(), event->getRequirements().end(), object) != event->getRequirements().end()) {
				return true;
			}
		}
	}
	return false;
}

void EventHandler::cancelRequirements(std::initializer_list<Object*> objects) {
	for (auto event : events) {
		for (auto object : objects) {
			if (find(event->getRequirements().begin(), event->getRequirements().end(), object) != event->getRequirements().end()) {
				event->cancel();
			}
		}
	}
}

void EventHandler::cancelRequirements(std::vector<Object*> objects) {
	for (auto event : events) {
		for (auto object : objects) {
			if (find(event->getRequirements().begin(), event->getRequirements().end(), object) != event->getRequirements().end()) {
				event->cancel();
			}
		}
	}
}

void EventHandler::scheduleEvent(Event* event) {
	event->schedule();
}
