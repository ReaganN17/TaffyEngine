#pragma once
#include "EventHeader.h"
#include "../EventHandler.h"

union EventByte {
	u8 byte = 0;

	struct {
		bool interruptable : 1;
		bool running : 1;
		bool linked : 1;
		bool free_bool : 1;
		u8 space : 4;
	};
};

struct Event {
	friend ParallelRace;
	friend ParallelEvent;
	friend ParallelDeadline;
	friend SequentialEvent;
	friend RepeatEvent;
	friend ConditionalEvent;
	friend EventHandler;

public:
	Event();
	
	virtual void schedule();
	virtual void cancel();

	bool isInterruptable();
	bool isRunning();
	bool isLinked();

	void setLink(bool link);
	void setInterrupted(bool interruptable);

	//dont use any of lambdas - still figuring it out
	ParallelEvent* alongWith(std::initializer_list<Event*> events);
	SequentialEvent* andThen(std::initializer_list<Event*> events);
	SequentialEvent* andThen(std::function<void()> run, std::initializer_list<Object*> requirements);
	SequentialEvent* beforeStarting(Event* before);
	SequentialEvent* beforeStarting(std::function<void()> before, std::initializer_list<Object*> requirements);
	ParallelDeadline* deadlineWith(std::initializer_list<Event*> events);
	ConditionalEvent* onlyIf(std::function<bool()> condition);
	ParallelRace* onlyWhile(std::function<bool()> condition);
	ParallelRace* raceWith(std::initializer_list<Event*> parallel);
	RepeatEvent* repeatedly();
	ConditionalEvent* unless(std::function<bool()> condition);
	ParallelRace* until(std::function<bool()> condition);
	ParallelRace* withTimeout(long ms);

	std::vector<Object*>& getRequirements();
protected:
	virtual void addRequirements(std::initializer_list<Object*> objs);
	virtual void addRequirements(std::vector<Object*> objs);
	virtual void addRequirements(Object* object);
	

	virtual void init() = 0;
	virtual void loop() = 0;
	virtual void end(bool interrupted) = 0;
	virtual bool isFinished() = 0;
	
private:
	EventByte eb;
	std::vector<Object*> requirements;
	
	virtual void kill();
	void run();
	virtual void destruct();

	void setRunning(bool run);

	void setBool(bool boo);
	bool getBool();
}; 