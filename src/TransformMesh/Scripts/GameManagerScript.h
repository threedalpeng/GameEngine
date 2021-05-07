#pragma once
#include <iostream>
#include "engine/Core.h"

class GameManagerScript : public Script
{
public:
	GameManagerScript() : Script() {}

	std::vector<GameObject*> dragons = std::vector<GameObject*>(3);
	std::vector<GameObject*> axis = std::vector<GameObject*>(2);
	Camera* camera = nullptr;

	void init() override {
		printHelp();
		camera = GameObject::find("Main Camera")->getComponent<Camera>();
		camera->activateTracking(dragons[0]->getComponent<Transform>(), vec3(0.f, 1.f, 0.f));
	}

	void update() override {
		Transform* transform = axis[0]->getComponent<Transform>();
		transform->rotate(0.f, 90.f * Time::delta(), 0.f);
		transform = axis[1]->getComponent<Transform>();
		transform->rotate(0.f, -90.f * Time::delta(), 0.f);

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

		if (Input::getKeyDown(GLFW_KEY_1)) {
			camera->activateTracking(dragons[0]->getComponent<Transform>(), vec3(0.f, 1.f, 0.f));
		}
		if (Input::getKeyDown(GLFW_KEY_2)) {
			camera->activateTracking(dragons[1]->getComponent<Transform>(), vec3(0.f, 1.f, 0.f));
		}
		if (Input::getKeyDown(GLFW_KEY_3)) {
			camera->activateTracking(dragons[2]->getComponent<Transform>(), vec3(0.f, 1.f, 0.f));
		}
		if (Input::getKeyDown(GLFW_KEY_4)) {
			camera->deactivateTracking();
		}

		if (Input::getKey(GLFW_KEY_LEFT_CONTROL)) {
			Transform* transform = dragons[0]->getComponent<Transform>();
			float moveScale = 10.f;
			if (Input::getKey(GLFW_KEY_E)) {
				transform->translate(Time::delta() * vec3(0.f, 1.f, 0.f) * moveScale);
			}
			if (Input::getKey(GLFW_KEY_Q)) {
				transform->position += Time::delta() * vec3(0.f, -1.f, 0.f) * moveScale;
			}
			if (Input::getKey(GLFW_KEY_W)) {
				transform->position += Time::delta() * vec3(0.f, 0.f, 1.f) * moveScale;
			}
			if (Input::getKey(GLFW_KEY_S)) {
				transform->position += Time::delta() * vec3(0.f, 0.f, -1.f) * moveScale;
			}
			if (Input::getKey(GLFW_KEY_A)) {
				transform->position += Time::delta() * vec3(1.f, 0.f, 0.f) * moveScale;
			}
			if (Input::getKey(GLFW_KEY_D)) {
				transform->position += Time::delta() * vec3(-1.f, 0.f, 0.f) * moveScale;
			}
		}
		if (Input::getKey(GLFW_KEY_LEFT_SHIFT)) {
			// heading: y, attitude: x, bank: z
			Transform* dtransform = dragons[0]->getComponent<Transform>();
			if (Input::getKeyDown(GLFW_KEY_X)) {
				dtransform->rotate(90, 0, 0);
			}
			else if (Input::getKeyDown(GLFW_KEY_Y)) {
				dtransform->rotate(0, 90, 0);
			}
			else if (Input::getKeyDown(GLFW_KEY_Z)) {
				dtransform->rotate(0, 0, 90);
			}
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
