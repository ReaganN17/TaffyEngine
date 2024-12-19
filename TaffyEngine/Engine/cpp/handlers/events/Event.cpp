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