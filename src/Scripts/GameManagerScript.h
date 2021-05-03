#pragma once
#include <iostream>
#include "engine/Input.h"
#include "engine/Time.h"
#include "engine/Script/Script.h"
#include "engine/Transform/Transform.h"

class GameManagerScript : public Script
{
public:
	GameManagerScript() : Script() {}

	std::vector<GameObject*> dragons = std::vector<GameObject*>(3);

	void init() override {
		printHelp();
	}

	void update() override {
		if (Input::getKeyDown(GLFW_KEY_KP_ADD) ||
			(Input::getKeyDown(GLFW_KEY_EQUAL) && Input::getKey(GLFW_KEY_LEFT_SHIFT))) {
			if (NUM_INSTANCE < MAX_INSTANCE)
				printf("> NUM_INSTANCE = % -4d\r", ++NUM_INSTANCE);
		}
		if (Input::getKeyDown(GLFW_KEY_KP_SUBTRACT) || Input::getKeyDown(GLFW_KEY_MINUS)) {
			if (MIN_INSTANCE < NUM_INSTANCE)
				printf("> NUM_INSTANCE = % -4d\r", --NUM_INSTANCE);
		}
		if (Input::getKeyDown(GLFW_KEY_H)) {
			printHelp();
		}
		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			dvec2 pos = Input::getCursorPoint();
			printf("> Left mouse button pressed at (%d, %d)\n", int(pos.x), int(pos.y));
		}

		if (Input::getKey(GLFW_KEY_W)) {
			Transform* transform = dragons[0]->getComponent<Transform>();
			transform->position += Time::delta() * vec3(0.f, 1.f, 0.f);
		}
	}

	void printHelp() {
		printf("[help]\n");
		printf("- press ESC or 'q' to terminate the program\n");
		printf("- press F1 or 'h' to see help\n");
		printf("- press '+/-' to increase/decrease the number of instances (min=%d, max=%d)\n", MIN_INSTANCE, MAX_INSTANCE);
		printf("\n");
	}
private:
	const uint MIN_INSTANCE = 1;
	const uint MAX_INSTANCE = 3;
	uint NUM_INSTANCE = 1;
};
