#pragma once
#include <iostream>

#include "engine/Scene.h"
#include "engine/Input.h"
#include "engine/Transform/Transform.h"
#include "engine/Script/ScriptLoader.h"
#include "DragonFactory.h"
#include "Scripts/GameManagerScript.h"

class TransformScene : public Scene {
public:
	TransformScene() : Scene() {
		vert_shader_path = "shaders/transform.vert";
		frag_shader_path = "shaders/transform.frag";
	};

	void init() {
		DragonFactory::init();

		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f); // set clear color

		GameObject* gameManager = GameObject::create("Game Manager");
		ScriptLoader* scriptLoader = gameManager->addComponent<ScriptLoader>();
		GameManagerScript* script = new GameManagerScript;
		scriptLoader->addScript(script);

		GameObject* dragon = DragonFactory::create();
		Transform* transform = dragon->getComponent<Transform>();
		transform->position = vec3(0.f, 0.f, 0.f);
		addObject(dragon);
		script->dragons[0] = dragon;
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		Transform* originTransform = transform;

		dragon = DragonFactory::create();
		transform = dragon->getComponent<Transform>();
		transform->position = vec3(100.f, 0.f, 100.f);
		addObject(dragon);
		script->dragons[1] = dragon;
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		dragon = DragonFactory::create();
		transform = dragon->getComponent<Transform>();
		transform->position = vec3(-100.f, 0.f, 100.f);
		addObject(dragon);
		script->dragons[2] = dragon;
		std::cout << "GameObject Created: " << dragon->name() << std::endl;

		GameObject* camera = GameObject::create("Main Camera");
		camera->addComponent<Camera>();
		transform = camera->getComponent<Transform>();
		transform->position = vec3(0, 30, 300);
		transform->lookAt(*originTransform);
	}

	void update() {
		Scene::update();
	}
};