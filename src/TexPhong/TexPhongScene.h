#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Scripts/CameraScript.h"
#include "Scripts/BustScript.h"

class TexPhongScene : public Scene {
public:
	TexPhongScene() : Scene() {
		// register shader path
		vert_shader_path = "shaders/texphong.vert";
		frag_shader_path = "shaders/texphong.frag";
	};

	void init() {
		Mesh* mesh = new Mesh();
		mesh->loadMesh("mesh/head.vertex.bin", "mesh/head.index.bin");
		mesh->loadTexture("mesh/head.jpg");

		Material* material = new Material();

		GameObject* bust = GameObject::create("Bust");
		Renderer* renderer = bust->addComponent<Renderer>();
		renderer->loadMesh(mesh);
		renderer->loadMaterial(material);
		bust->addComponent<ScriptLoader>()->addScript(new BustScript());
		addObject(bust);

		GameObject* mainCamera = GameObject::create("Main Camera");
		mainCamera->getComponent<Transform>()->translate(30, -100, 10);
		Camera* camera = mainCamera->addComponent<Camera>();
		CameraScript* script = new CameraScript();
		script->trackingTarget = bust;
		mainCamera->addComponent<ScriptLoader>()->addScript(script);
		camera->setThisMainCamera();
		addObject(mainCamera);

		GameObject* directionalLight = GameObject::create("Directional Light");
		Transform* transform = directionalLight->getComponent<Transform>();
		transform->translate(1.0f, -1.f, 1.f);
		directionalLight->addComponent<Light>();
		addObject(directionalLight);
	}
};