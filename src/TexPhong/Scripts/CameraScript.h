#pragma once
#include <iostream>
#include "engine/Core.h"

class CameraScript : public Script
{
public:
	CameraScript() : Script() {}

	GameObject* trackingTarget = nullptr;

	void init() override {
		printHelp();
	}

	void update() override {
		getComponent<Camera>()->activateTracking(trackingTarget->getComponent<Transform>(), vec3(0, 1, 0));
	}

	void printHelp() {
		printf("[help]\n");
		printf("- press ESC or 'q' to terminate the program\n");
		printf("- press F1 or 'h' to see help\n");
		printf("\n");
	}
};
