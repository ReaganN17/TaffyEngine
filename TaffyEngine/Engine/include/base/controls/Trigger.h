#pragma once

#include "../../TaffyEngine.h"

class Controls;

enum TriggerType : u8 {
	ON_CHANGE, ON_TRUE, ON_FALSE,
	WHILE_TRUE, WHILE_FALSE, 
	TOGGLE_ON_TRUE, TOGGLE_ON_FALSE
};

class Trigger final {
	friend Controls;

	std::function<Event* ()> factory = []() {return new WaitUntil((long)0); };
	Event* event = nullptr;
	u8 button;
	TriggerType type;

	void update();
	void initialize();
	void allocateEvent();
	void deallocateEvent();
public: 
	Trigger(u8 button, TriggerType type, std::function<Event* ()> factory);
	~Trigger();

	
};