#include "include/WindowTime.h"

double WindowTime::currentTimeValue;
double WindowTime::lastTimeValue;
double WindowTime::deltaTimeValue;
int WindowTime::frameCount;
int WindowTime::framePerSecond;

void WindowTime::updateTimeValue() {
	lastTimeValue = currentTimeValue;
	currentTimeValue = glfwGetTime();
	deltaTimeValue = currentTimeValue - lastTimeValue;
	++frameCount;
	framePerSecond = 1.0 / deltaTimeValue;
}