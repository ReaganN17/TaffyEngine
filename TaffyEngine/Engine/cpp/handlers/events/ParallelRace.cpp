#include "../../../include/TaffyEngine.h"

/*
ParallelRace

Group of events, but only this one itself is "scheduled" to the Event handler
Runs until first event done, then all events will cancel (also means interruptibility is null)

*/

ParallelRace::ParallelRace() {}
ParallelRace::ParallelRace(std::initializer_list<Event*> events) { addEvents(events); }
ParallelRace::ParallelRace(Event* event, std::initializer_list<Event*> events) { addEvents(events); addEvents({ event }); }

void ParallelRace::addEvents(std::initializer_list<Event*> events) {
	this->events.insert(this->events.end(), events.begin(), events.end());

	std::sort(this->events.begin(), this->events.end());
	this->events.erase(std::unique(this->events.begin(), this->events.end()), this->events.end());

	for (auto event : events) {
		addRequirements(event->getRequirements());
	}
}

void ParallelRace::init() {
	for (auto event : events) {
		event->init();
		event->setRunning(true);
	}
}

void ParallelRace::loop() {
	for (auto event : events) {
		if (event->isRunning()) event->loop();
	}
}

void ParallelRace::end(bool interrupted) {
	for (auto event : events) {
		if (event->isRunning()) event->end(true);
		event->setRunning(false);
	}
}

bool ParallelRace::isFinished() {
	for (auto event : events) {
		if (event->isFinished()) {
			event->end(false);
			event->setRunning(false);

			return true;
		}
	}
	return false;
}

void ParallelRace::destruct() {
	for (auto event : events) {
		event->destruct();
	}

	Event::destruct();
}