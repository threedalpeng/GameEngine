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

	/* Set initial state of scene */
	/* Don't operate any function in here, just add and link objects and scripts */
	void init() {
		DragonFactory::init();

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
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		//addObject(dragon);
		script->dragons[1] = dragon;

		axis->addChildren(dragon);
		addObject(axis);
		script->axis[0] = axis;

		// Axis for Dragon 3
		axis = GameObject::create("Axis");

		// Dragon 3
		dragon = DragonFactory::create();
		transform = dragon->getComponent<Transform>();
		transform->position = vec3(-100.f, 0.f, 100.f);
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		//addObject(dragon);
		script->dragons[2] = dragon;

		axis->addChildren(dragon);
		addObject(axis);
		script->axis[1] = axis;

		// Main Camera
		GameObject* camera = GameObject::create("Main Camera");
		Camera* cameraCmp = camera->addComponent<Camera>();
		cameraCmp->setThisMainCamera();
		transform = camera->getComponent<Transform>();
		transform->position = vec3(0, 30, 300);

		addObject(camera);
	}
};