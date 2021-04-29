#pragma once
#include "engine/Scene.h"
#include "engine/Input.h"
#include "engine/Transform/Transform.h"
#include "DragonFactory.h"

class TransformScene : public Scene {
public:
	TransformScene() : Scene() {};

	void init() {
		GameObject* dragon = DragonFactory::create();
		Transform* transform = dragon->addComponent<Transform>();
		transform->translate(0.f, 0.f, 0.f);
		addObject(dragon);

		dragon = DragonFactory::create();
		transform = dragon->addComponent<Transform>();
		transform->translate(100.f, 0.f, 100.f);
		addObject(dragon);

		dragon = DragonFactory::create();
		transform = dragon->addComponent<Transform>();
		transform->translate(-100.f, 0.f, 100.f);
		addObject(dragon);
	}

	void update() {
		if (Input::getKeyDown(GLFW_KEY_KP_ADD) ||
			(Input::getKeyDown(GLFW_KEY_EQUAL) && Input::getKey(GLFW_KEY_LEFT_SHIFT))) {
			if (NUM_INSTANCE < MAX_INSTANCE)
				printf("> NUM_INSTANCE = % -4d\r", ++NUM_INSTANCE);
		}

		if (Input::getKeyDown(GLFW_KEY_KP_SUBTRACT) || Input::getKeyDown(GLFW_KEY_MINUS)) {
			if (MIN_INSTANCE < NUM_INSTANCE)
				printf("> NUM_INSTANCE = % -4d\r", --NUM_INSTANCE);
		}

		if (Input::getKeyDown(GLFW_KEY_ESCAPE) || Input::getKeyDown(GLFW_KEY_Q)) {
			printHelp();
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