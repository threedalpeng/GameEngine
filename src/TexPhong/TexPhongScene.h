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

		Mesh* box = create_box_mesh();

		Material* material = new Material();

		GameObject* bust = GameObject::create("Bust");
		Renderer* renderer = bust->addComponent<Renderer>();
		renderer->loadMesh(mesh);
		renderer->loadMaterial(material);
		bust->addComponent<ScriptLoader>()->addScript(new BustScript());
		addObject(bust);

		GameObject* trackball = GameObject::create("Trackball");

		GameObject* mainCamera = GameObject::create("Main Camera");
		mainCamera->getComponent<Transform>()->translate(0, 0, 100);
		Camera* camera = mainCamera->addComponent<Camera>();
		CameraScript* script = new CameraScript();
		script->trackingTarget = bust;
		script->trackball = trackball;
		mainCamera->addComponent<ScriptLoader>()->addScript(script);
		camera->setThisMainCamera();

		addObject(trackball);
		trackball->addChildren(mainCamera);

		GameObject* directionalLight = GameObject::create("Directional Light");
		Transform* transform = directionalLight->getComponent<Transform>();
		transform->translate(1.0f, -1.f, 1.f);
		directionalLight->addComponent<Light>();
		addObject(directionalLight);

		GameObject* xAxis = GameObject::create("X Axis");
		transform = xAxis->getComponent<Transform>();
		renderer = xAxis->addComponent<Renderer>();
		renderer->loadMesh(box);
		//renderer->loadMaterial(material);
		transform->translate(50.f, 0, 0);
		transform->scale = vec3(100.f, 1.f, 1.f);
		addObject(xAxis);

		GameObject* yAxis = GameObject::create("Y Axis");
		transform = yAxis->getComponent<Transform>();
		renderer = yAxis->addComponent<Renderer>();
		renderer->loadMesh(box);
		//renderer->loadMaterial(material);
		transform->scale = vec3(100.f, 1.f, 1.f);
		transform->rotate(vec3(0, 0, 1), 90.f);
		transform->translate(0.f, 50.f, 0);
		addObject(yAxis);

		GameObject* vAxis = GameObject::create("Virtual Axis");
		transform = vAxis->getComponent<Transform>();
		renderer = vAxis->addComponent<Renderer>();
		renderer->loadMesh(box);
		renderer->loadMaterial(material);
		transform->scale = vec3(10.f, 10.f, 10.f);
		script->vAxis = vAxis;
		addObject(vAxis);
	}

	Mesh* create_box_mesh() {
		Mesh* mesh = new Mesh();

		mesh->vertex_buffer = 0;
		mesh->index_buffer = 0;

		mesh->vertex_list.push_back({ vec3(0.5f, 0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, 0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, 0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(0.5f, 0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(0.5f, -0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, -0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, -0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(0.5f, -0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });

		mesh->index_list.push_back(1);
		mesh->index_list.push_back(2);
		mesh->index_list.push_back(4);

		mesh->index_list.push_back(1);
		mesh->index_list.push_back(4);
		mesh->index_list.push_back(5);

		mesh->index_list.push_back(1);
		mesh->index_list.push_back(5);
		mesh->index_list.push_back(2);

		mesh->index_list.push_back(6);
		mesh->index_list.push_back(7);
		mesh->index_list.push_back(2);

		mesh->index_list.push_back(6);
		mesh->index_list.push_back(5);
		mesh->index_list.push_back(7);

		mesh->index_list.push_back(6);
		mesh->index_list.push_back(2);
		mesh->index_list.push_back(5);

		mesh->index_list.push_back(8);
		mesh->index_list.push_back(5);
		mesh->index_list.push_back(4);

		mesh->index_list.push_back(8);
		mesh->index_list.push_back(4);
		mesh->index_list.push_back(7);

		mesh->index_list.push_back(8);
		mesh->index_list.push_back(7);
		mesh->index_list.push_back(5);

		mesh->index_list.push_back(3);
		mesh->index_list.push_back(4);
		mesh->index_list.push_back(2);

		mesh->index_list.push_back(3);
		mesh->index_list.push_back(2);
		mesh->index_list.push_back(7);

		mesh->index_list.push_back(3);
		mesh->index_list.push_back(7);
		mesh->index_list.push_back(4);

		glGenBuffers(1, &(mesh->vertex_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);
		glGenBuffers(1, &(mesh->index_buffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

		mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
		return mesh;
	}
};