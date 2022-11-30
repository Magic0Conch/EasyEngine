#pragma once
#include <glfw3.h>
class WindowTime
{
public:
	static double deltaTimeValue;
	static double lastTimeValue;
	static double currentTimeValue;
	static int frameCount;
	static int framePerSecond;
	static void updateTimeValue();
};

