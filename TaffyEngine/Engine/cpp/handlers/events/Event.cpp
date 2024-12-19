#include "../../../include/TaffyEngine.h"

/*
Event cpp

Basic Event structure
- init, loop, end, isFinished are the main inherited methods
- [fill in later]
*/

Event::Event() {
	eb.interruptable = true;
}

void Event::destruct() { if (eb.linked) delete this; }

void Event::schedule() {
	if (EventHandler::getInstance().addEvent(this)) {
		eb.running = true;
		init();
	}
	else {
		destruct();
	}
}

//overrides interruptability
void Event::cancel() {
	eb.running = false;
	end(true);
	EventHandler::getInstance().removeEvent(this);

	destruct();
}

bool Event::isInterruptable() { return eb.interruptable; }
bool Event::isRunning() { return eb.running; }
bool Event::isLinked() { return eb.linked; }
bool Event::getBool() { return eb.free_bool; }

void Event::setLink(bool link) { eb.linked = link; }
void Event::setInterrupted(bool interruptable) { eb.interruptable = interruptable; }
void Event::setRunning(bool run) { eb.running = run; }
void Event::setBool(bool boo) { eb.free_bool = boo; }

void Event::addRequirements(std::initializer_list<Object*> objs) {
	requirements.insert(requirements.end(), objs.begin(), objs.end());

	std::sort(requirements.begin(), requirements.end());
	requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());
}
void Event::addRequirements(std::vector<Object*> objs) {
	requirements.insert(requirements.end(), objs.begin(), objs.end());

	std::sort(requirements.begin(), requirements.end());
	requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());
}
void Event::addRequirements(Object* object) {
	if (std::find(requirements.begin(), requirements.end(), object) != requirements.end()) requirements.push_back(object);
}

std::vector<Object*>& Event::getRequirements() {return requirements;}

void Event::run() {
	if (isFinished()) { kill(); }
	loop();


	
}

void Event::kill() {
	eb.running = false;
	end(false);
	EventHandler::getInstance().removeEvent(this);

	//self destruct
	destruct();
}

ParallelEvent* Event::alongWith(std::initializer_list<Event*> events) {
	return new ParallelEvent(this, events);
}
SequentialEvent* Event::andThen(std::initializer_list<Event*> events) {
	return new SequentialEvent(this, events);
}
SequentialEvent* Event::andThen(void (*run)(), std::initializer_list<Object*> requirements) {
	return new SequentialEvent({ this, new InstantEvent(run, requirements) });
}
SequentialEvent* Event::beforeStarting(Event* before) {
	return new SequentialEvent({ before, this });
}
SequentialEvent* Event::beforeStarting(void (*before)(), std::initializer_list<Object*> requirements) {
	return new SequentialEvent({ new InstantEvent(before, requirements), this});
}
ParallelDeadline* Event::deadlineWith(std::initializer_list<Event*> events) {
	return new ParallelDeadline(this, events);
}
ConditionalEvent* Event::onlyIf(bool (*condition)()) {
	return new ConditionalEvent(this, new WaitUntil((long)0), condition);
}
ParallelRace* Event::onlyWhile(bool (*condition)()) {
	return new ParallelRace({ this, new WaitUntil(condition) });
}
ParallelRace* Event::raceWith(std::initializer_list<Event*> parallel) {
	return new ParallelRace(this, parallel);
}
RepeatEvent* Event::repeatedly() {
	return new RepeatEvent(this);
}
ConditionalEvent* Event::unless(bool (*condition)()) {
	return new ConditionalEvent(new WaitUntil((long)0), this, condition);
}
ParallelRace* Event::until(bool (*condition)()) {
	return new ParallelRace({ this, new WaitUntil([]() {return false; }) });//place holder - looking into lambdas a bit more
}
ParallelRace* Event::withTimeout(long ms) {
	return new ParallelRace({ this, new WaitUntil(ms) });
}




/*
ParallelRace

Group of events, but only this one itself is "scheduled" to the Event handler
Runs until first event done, then all events will cancel (also means interruptibility is null)

*/

ParallelRace::ParallelRace() {}
ParallelRace::ParallelRace(std::initializer_list<Event*> events) {addEvents(events);}
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

/*
ParallelDeadline

Group of events, but only this one itself is "scheduled" to the Event handler
Runs until deadline event done, then all events will cancel (also means interruptibility is null)

*/
ParallelDeadline::ParallelDeadline() {}
ParallelDeadline::ParallelDeadline(Event* deadline, std::initializer_list<Event*> events) { 
	this->deadline == deadline;
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

void ParallelDeadline::init() {
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

/*
ParallelDeadline

Group of events, but only this one itself is "scheduled" to the Event handler
All events run at the same time, ending at different times
Ends when all events have finished
*/
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

/*
SequentialEvent

Group of events, but only this one itself is "scheduled" to the Event handler
Events run in order, when one ends, the next one starts
(avoid using non self terminating commands)
*/

SequentialEvent::SequentialEvent() {}
SequentialEvent::SequentialEvent(std::initializer_list<Event*> events) { addEvents(events); }
SequentialEvent::SequentialEvent(Event* first, std::initializer_list<Event*> following) { addEvents({ first }); addEvents(following); }

void SequentialEvent::addEvents(std::initializer_list<Event*> events) {
	this->events.insert(this->events.begin(), events.begin(), events.end());

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

/*
RepeatEvent

Frame for the repeating event
when it ends, it inits and starts running again

ends when canceled
*/

RepeatEvent::RepeatEvent(Event* event) {
	this->event = event;

	addRequirements(event->getRequirements());
}

void RepeatEvent::init() {
	event->init();
	event->setRunning(true);
}
void RepeatEvent::loop() {
	event->loop();

	if (event->isFinished()) {
		event->end(true);
		event->init();
	}
}
void RepeatEvent::end(bool interrupted) {
	event->end(true);
}
bool RepeatEvent::isFinished() {
	return false;
}

void RepeatEvent::destruct() {
	event->destruct();

	Event::destruct();
}

/*
Functional Event

Event made by lambdas
*/

FunctionalEvent::FunctionalEvent(void (*init)(), void (*loop)(), void (*end)(), bool(*isFinshed)(), std::initializer_list<Object*> requirements) {
	this->init_lamb = init;
	this->loop_lamb = loop;
	this->end_lamb = end;
	this->isFinished_lamb = isFinshed;
	addRequirements(requirements);
}
void FunctionalEvent::init() {
	if (init_lamb != nullptr) init_lamb();
}

void FunctionalEvent::loop() {
	if (loop_lamb != nullptr) loop_lamb();
}
void FunctionalEvent::end(bool interrupted) {
	if (end_lamb != nullptr) this->end_lamb();
}
bool FunctionalEvent::isFinished() {
	if (isFinished_lamb != nullptr) this->isFinished_lamb();

	return false;
}

/*
ConditionalEvent

this or that given a boolean method
*/
ConditionalEvent::ConditionalEvent(Event* onTrue, Event* onFalse, bool(*condition)()) {
	on_true = onTrue;
	on_false = onFalse;
	this->condition = condition;

	addRequirements(on_true->getRequirements());
	addRequirements(on_false->getRequirements());
}

void ConditionalEvent::init() {
	setBool(condition());

	if (getBool()) {
		if (on_true != nullptr) {
			on_true->init();
			on_true->setRunning(true);
		}
	}
	else {
		on_false->init();
	}
}

void ConditionalEvent::loop() {
	if (getBool()) {
		on_true->loop();
	}
	else {
		on_false->loop();
	}
}

void ConditionalEvent::end(bool interrupted) {
	if (getBool()) {
		on_true->end(interrupted);
	}
	else {
		on_false->end(interrupted);
	}
}
bool ConditionalEvent::isFinished() {
	if (getBool()) {
		return on_true->isFinished();
	}
	else {
		return on_false->isFinished();
	}
}

void ConditionalEvent::destruct() {
	on_true->destruct();
	on_false->destruct();

	Event::destruct();
}
/*
InstantEvent

method that runs on init then ends instantly
*/

InstantEvent::InstantEvent(void (*init)(), std::initializer_list<Object*> requirements)
	: FunctionalEvent(init, nullptr, nullptr, [](){return true; }, requirements) {}

/*
RunEvent

method that runs and continues to run
*/
RunEvent::RunEvent(void (*loop)(), std::initializer_list<Object*> requirements)
	: FunctionalEvent(nullptr, loop, nullptr, []() {return false; }, requirements)
{}







