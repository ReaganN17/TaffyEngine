#include "../../include/utils/Utils.h"

Timer::Timer() {}

void Timer::start() {
	start_time = GetTickCount();
}

long Timer::getTime() {
	return GetTickCount() - start_time;
}