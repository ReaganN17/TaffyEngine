#pragma once

#include "../../TaffyEngine.h"

class Trigger;

class Controls final{
	static Controls instance;
	std::vector<Trigger*> triggers;
public:
	static Controls& getInstance();

	void initialize();
	void update();
	void addTrigger(Trigger* trigger);
	void clearTrigger(u8 button);
	void clearControls();
};