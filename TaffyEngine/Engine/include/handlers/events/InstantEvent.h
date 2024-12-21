#pragma once
#include "EventHeader.h"
struct InstantEvent final : FunctionalEvent {
	InstantEvent(std::function<void()> init, std::initializer_list<Object*> requirements);
};