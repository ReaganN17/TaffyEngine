#pragma once

//units in milliseconds
struct Timer {
private:
	long start_time = 0;
public:
	Timer();

	void start();
	long getTime();
};