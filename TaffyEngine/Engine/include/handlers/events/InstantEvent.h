#pragma once
#include "EventHeader.h"
struct InstantEvent final : FunctionalEvent {
	InstantEvent(void (*init)(), std::initializer_list<Object*> requirements);
};