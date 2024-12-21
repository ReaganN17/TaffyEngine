#include "../../../include/TaffyEngine.h"


/*
RunEvent

method that runs and continues to run
*/
RunEvent::RunEvent(std::function<void()> loop, std::initializer_list<Object*> requirements)
	: FunctionalEvent([]() {}, loop, []() {}, []() {return false; }, requirements)
{
}