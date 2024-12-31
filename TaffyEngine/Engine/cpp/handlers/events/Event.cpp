#include "../../../include/handlers/events/Event.h"

#include "../../../include/handlers/events/EventHeader.h"

Event::Event() {}

void Event::destruct() { if (linked) delete this; }

void Event::schedule() {
	if (EventHandler::addEvent(this)) {
		running = true;
		init();
	}
}

void Event::cancel() {
	running = false;
	end(true);
	EventHandler::removeEvent(this);

	destruct();
}

bool Event::isRunning() { return running; }
bool Event::isLinked() { return linked; }

void Event::setLink(bool link) { linked = link; }
void Event::setRunning(bool run) { running = run; }

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
	running = false;
	end(false);
	EventHandler::removeEvent(this);

	//self destruct
	destruct();
}

ParallelEvent* Event::alongWith(std::initializer_list<Event*> events) {
	return new ParallelEvent(this, events);
}
SequentialEvent* Event::andThen(std::initializer_list<Event*> events) {
	return new SequentialEvent(this, events);
}
SequentialEvent* Event::andThen(std::function<void()> run, std::initializer_list<Object*> requirements) {
	return new SequentialEvent({ this, new InstantEvent(run, requirements) });
}
SequentialEvent* Event::beforeStarting(Event* before) {
	return new SequentialEvent({ before, this });
}
SequentialEvent* Event::beforeStarting(std::function<void()> before, std::initializer_list<Object*> requirements) {
	return new SequentialEvent({ new InstantEvent(before, requirements), this});
}
ParallelDeadline* Event::deadlineWith(std::initializer_list<Event*> events) {
	return new ParallelDeadline(this, events);
}
ConditionalEvent* Event::onlyIf(std::function<bool()> condition) {
	return new ConditionalEvent(this, new WaitUntil((long)0), condition);
}
ParallelRace* Event::onlyWhile(std::function<bool()> condition) {
	return new ParallelRace({ this, new WaitUntil([condition]() {return !condition(); }) });
}
ParallelRace* Event::raceWith(std::initializer_list<Event*> parallel) {
	return new ParallelRace(this, parallel);
}
RepeatEvent* Event::repeatedly() {
	return new RepeatEvent(this);
}
ConditionalEvent* Event::unless(std::function<bool()> condition) {
	return new ConditionalEvent(new WaitUntil((long)0), this, condition);
}
ParallelRace* Event::until(std::function<bool()> condition) {
	return new ParallelRace({ this, new WaitUntil(condition) });
}
ParallelRace* Event::withTimeout(long ms) {
	return new ParallelRace({ this, new WaitUntil(ms) });
}