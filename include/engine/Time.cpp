#include "Time.h"

float Time::_delta = 0.f;
float Time::_prevTime = 0.f;
float Time::_lag = 0.f;
float Time::_fixedTime = 0.f;

void Time::init() {
	_prevTime = currentTime();
}

float Time::delta() {
	return _delta;
}

float Time::currentTime() {
	return static_cast<float>(glfwGetTime());
}

bool Time::needsFixedUpdate() {
	bool ret = _lag >= _fixedTime;
	_lag -= _fixedTime;
	return ret;
}

void Time::updateDelta() {
	float t = currentTime();
	_delta = t - _prevTime;
	_prevTime = t;
	_lag += _delta;
}

void Time::setFixedUpdateRate(float rate) {
	_fixedTime = 1.f / rate;
}