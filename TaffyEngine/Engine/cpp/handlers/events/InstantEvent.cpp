#include "../../../include/TaffyEngine.h"
/*
InstantEvent

method that runs on init then ends instantly
*/

InstantEvent::InstantEvent(std::function<void()> init, std::initializer_list<Object*> requirements)
	: FunctionalEvent(init, []() {}, []() {}, []() {return true; }, requirements) {
}