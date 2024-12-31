#include "../../../include/handlers/events/SequentialEvent.h"

SequentialEvent::SequentialEvent() {}
SequentialEvent::SequentialEvent(std::initializer_list<Event*> events) { addEvents(events); }
SequentialEvent::SequentialEvent(Event* first, std::initializer_list<Event*> following) { addEvents({ first }); addEvents(following); }

void SequentialEvent::addEvents(std::initializer_list<Event*> events) {
	this->events.insert(this->events.end(), events.begin(), events.end());

	for (auto event : events) {
		addRequirements(event->getRequirements());
	}
}
void SequentialEvent::init() {
	events.front()->init();
	events.front()->setRunning(true);
}
void SequentialEvent::loop() {
	if (iterator == events.size()) return;
	Event* cur_event = events.at(iterator);

	cur_event->loop();

	if (cur_event->isFinished()) {
		cur_event->end(false);
		cur_event->setRunning(false);

		if (++iterator < events.size()) {
			events.at(iterator)->init();
			events.at(iterator)->setRunning(true);
		}
	}
}
void SequentialEvent::end(bool interrupted) {
	if (interrupted) {
		for (auto event : events) {
			if (event->isRunning()) event->end(true);
		}
	}
}
bool SequentialEvent::isFinished() {
	return iterator == events.size();
}

void SequentialEvent::destruct() {
	for (auto event : events) {
		event->destruct();
	}

	Event::destruct();
}