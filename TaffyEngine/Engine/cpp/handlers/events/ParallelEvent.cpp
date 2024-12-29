#include "../../../include/handlers/events/ParallelEvent.h"

ParallelEvent::ParallelEvent() {}
ParallelEvent::ParallelEvent(std::initializer_list<Event*> events) { addEvents(events); }
ParallelEvent::ParallelEvent(Event* event, std::initializer_list<Event*> other_events) { addEvents(other_events); addEvents({ event }); }

void ParallelEvent::addEvents(std::initializer_list<Event*> events) {
	this->events.insert(this->events.begin(), events.begin(), events.end());

	std::sort(this->events.begin(), this->events.end());
	this->events.erase(std::unique(this->events.begin(), this->events.end()), this->events.end());

	for (auto event : events) {
		addRequirements(event->getRequirements());
	}
}

void ParallelEvent::init() {
	for (auto event : events) {
		event->init();
		event->setRunning(true);
	}
}
void ParallelEvent::loop() {
	for (auto event : events) {
		if (event->isRunning())event->loop();

		if (event->isRunning() && event->isFinished()) {
			event->end(false);
			event->setRunning(false);
		}
	}
}
void ParallelEvent::end(bool interrupted) {
	if (interrupted) {
		for (auto event : events) {
			if (event->isRunning()) event->end(true);
		}
	}
}
bool ParallelEvent::isFinished() {
	for (auto event : events) {
		if (event->isRunning()) return false;
	}
	return true;
}

void ParallelEvent::destruct() {
	for (auto event : events) {
		event->destruct();
	}

	Event::destruct();
}