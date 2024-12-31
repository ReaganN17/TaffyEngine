#include "../../include/utils/Timer.h"

Timer::Timer() {}

void Timer::start() {
	start_time = GetTickCount();
}

long Timer::getTime() {
	return GetTickCount() - start_time;
}

bool Timer::hasElapsed(long time) {
	return getTime() >= time;
}