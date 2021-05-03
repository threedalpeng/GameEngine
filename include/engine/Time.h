#pragma once
#include "gl/glfw/glfw3.h"

class Time
{
public:
	static void init();
	static float delta();
	static float currentTime();
	static bool needsFixedUpdate();
	static void updateDelta();
	static void setFixedUpdateRate(float rate);
private:
	static float _delta;
	static float _prevTime;
	static float _lag;
	static float _fixedTime;
};
