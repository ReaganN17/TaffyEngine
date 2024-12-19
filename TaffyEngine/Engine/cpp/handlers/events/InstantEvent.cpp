#include "../../../include/TaffyEngine.h"
/*
InstantEvent

method that runs on init then ends instantly
*/

InstantEvent::InstantEvent(void (*init)(), std::initializer_list<Object*> requirements)
	: FunctionalEvent(init, nullptr, nullptr, []() {return true; }, requirements) {
}