#pragma once
#include <chrono>

typedef std::chrono::time_point<std::chrono::steady_clock> timer_time_point;

struct timer
{
	timer_time_point TimeStart;
	double DurationMiliSeconds;
};

void StartTimer(timer* Timer) {
	Timer->TimeStart = std::chrono::steady_clock::now();
}
bool CheckTimerEnd(timer* Timer) {
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - Timer->TimeStart).count() > Timer->DurationMiliSeconds)
	{
		return true;
	}
	else
	{
		return false;
	}
}