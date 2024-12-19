#include "../../../include/TaffyEngine.h"


/*
RunEvent

method that runs and continues to run
*/
RunEvent::RunEvent(void (*loop)(), std::initializer_list<Object*> requirements)
	: FunctionalEvent(nullptr, loop, nullptr, []() {return false; }, requirements)
{
}