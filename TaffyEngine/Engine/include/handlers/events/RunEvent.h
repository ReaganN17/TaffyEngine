#pragma once
#include "EventHeader.h"
struct RunEvent final : FunctionalEvent {
	RunEvent(void (*init)(), std::initializer_list<Object*> requirements);
};
