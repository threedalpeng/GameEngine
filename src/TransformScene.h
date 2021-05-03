#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "DragonFactory.h"
#include "Scripts/GameManagerScript.h"

class TransformScene : public Scene {
public:
	TransformScene() : Scene() {
		// register shader path
		vert_shader_path = "shaders/transform.vert";
		frag_shader_path = "shaders/transform.frag";
	};

	void init() {
		DragonFactory::init();

		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f); // set clear color

		// Object for storing game mananging script
		GameObject* gameManager = GameObject::create("Game Manager");
		ScriptLoader* scriptLoader = gameManager->addComponent<ScriptLoader>();
		GameManagerScript* script = new GameManagerScript;
		scriptLoader->addScript(script);

		// Dragon 1
		GameObject* dragon = DragonFactory::create();
		Transform* transform = dragon->getComponent<Transform>();
		transform->position = vec3(0.f, 0.f, 0.f);
		addObject(dragon);
		script->dragons[0] = dragon;
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		Transform* originTransform = transform;

		// Axis for Dragon 2
		GameObject* axis = GameObject::create("Axis");

		// Dragon 2
		dragon = DragonFactory::create();
		transform = dragon->getComponent<Transform>();
		transform->position = vec3(100.f, 0.f, 100.f);
		script->dragons[1] = dragon;
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		axis->addChildren(dragon);
		addObject(axis);
		script->axis[0] = axis;

		// Axis for Dragon 3
		axis = GameObject::create("Axis");

		// Dragon 3
		dragon = DragonFactory::create();
		transform = dragon->getComponent<Transform>();
		transform->position = vec3(-100.f, 0.f, 100.f);
		script->dragons[2] = dragon;
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		axis->addChildren(dragon);
		addObject(axis);
		script->axis[1] = axis;

		// Main Camera
		GameObject* camera = GameObject::create("Main Camera");
		Camera* cameraCmp = camera->addComponent<Camera>();
		cameraCmp->setThisMainCamera();
		transform = camera->getComponent<Transform>();
		transform->position = vec3(0, 30, 300);
		transform->lookAt(*originTransform);
	}
};