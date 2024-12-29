#include "../../../include/handlers/events/ParallelDeadline.h"

ParallelDeadline::ParallelDeadline() {};

ParallelDeadline::ParallelDeadline(Event* deadline, std::initializer_list<Event*> events) {
	this->deadline = deadline;
	addRequirements(deadline->getRequirements());
	addEvents(events);
}

void ParallelDeadline::addEvents(std::initializer_list<Event*> events) {
	this->events.insert(this->events.end(), events.begin(), events.end());

	std::sort(this->events.begin(), this->events.end());
	this->events.erase(std::unique(this->events.begin(), this->events.end()), this->events.end());

	for (auto event : events) {
		addRequirements(event->getRequirements());
	}
}

void ParallelDeadline::setDeadline(Event* deadline) {
	this->deadline = deadline;
}

void ParallelDeadline::init() {
	if (deadline == nullptr) deadline == new WaitUntil((long)0);

	deadline->init();
	deadline->setRunning(true);

	for (auto event : events) {
		event->init();
		event->setRunning(true);
	}
}

void ParallelDeadline::loop() {
	deadline->loop();

	for (auto event : events) {
		if (event->isRunning()) event->loop();

		if (event->isRunning() && event->isFinished()) {
			event->end(true);
			event->setRunning(false);
		}
	}
}
void ParallelDeadline::end(bool interrupted) {
	deadline->end(interrupted);
	deadline->setRunning(false);

	for (auto event : events) {
		if (event->isRunning()) event->end(true);
		event->setRunning(false);
	}
}
bool ParallelDeadline::isFinished() {
	return deadline->isFinished();
}

void ParallelDeadline::destruct() {
	for (auto event : events) {
		event->destruct();
	}

	Event::destruct();
}