#pragma once
#include "EventHeader.h"
struct RunEvent final : FunctionalEvent {
	RunEvent(std::function<void()> loop, std::initializer_list<Object*> requirements);
};
