#pragma once
#include "engine/Core.h"

class BustScript : public Script
{
public:
	BustScript() : Script() {}

	void init() override {
	}

	void update() override {
		/*
		Transform* transform = getComponent<Transform>();
		float scale = 1.0f + float(cosf(Time::currentTime() * 1.5f)) * 0.05f;
		transform->scale = vec3(scale);
		*/
	}
};