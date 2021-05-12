#include <iostream>

#include "engine/Application.h"
#include "engine/Graphics/Renderer.h"
#include "engine/Graphics/Light.h"
#include "engine/Graphics/Camera.h"
#include "engine/Transform/Transform.h"
#include "engine/Script/ScriptLoader.h"
#include "engine/Time.h"

void iterateTransform(GameObject* obj) {
	Transform* transform = obj->getComponent<Transform>();
	transform->update();
	auto children = obj->getChildren();
	if (children.empty()) return;
	for (auto child : children) {
		iterateTransform(child.get());
	}
}

Application::Application(std::string title, ivec2 window_size)
	: _title(title), _window_size(window_size) {}

void Application::run()
{
	init();
	Time::init();
	Time::setFixedUpdateRate(_frame_rate);

	for (_frame_count = 0; !glfwWindowShouldClose(_window); _frame_count++) {
		Time::updateDelta();

		glfwPollEvents();
		Input::poll();

		// fixed time update
		while (Time::needsFixedUpdate()) {
			fixedUpdate();
		}

		if (_frame_count % 20 == 0) {
			printf("                           \r");
			printf(" FPS: %f\r", 1 / Time::delta());
		}
		update();
		render();
	}

	terminate();
}

void Application::init()
{
	if (!(_window = cg_create_window(_title.c_str(), _window_size.x, _window_size.y))) {
		glfwTerminate();
		exit(1);
	}

	if (!cg_init_extensions(_window)) {
		glfwTerminate();
		exit(1);
	}

	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int w, int h) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->reshape(window, w, h);
		});
	glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->keyboard(window, key, scancode, action, mods);
		});
	glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->mouse(window, button, action, mods);
		});
	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->motion(window, x, y);
		});

	ServiceLocator::provide<ComponentManager>(&_componentManager);
}

void Application::loadScene(Scene* scene)
{
	_current_scene = scene;

	// Initiate Managers By Scene
	shader.loadFrom(scene->vert_shader_path, scene->frag_shader_path);

	_current_scene->init();

	for (auto rootObj : _current_scene->getRootObjects()) {
		iterateTransform(rootObj);
	}

	if (auto componentList = _componentManager.getComponentList<ScriptLoader>()) {
		for (auto componentPair : *componentList) {
			ScriptLoader* scriptLoader = componentPair.second.get();
			for (auto script : scriptLoader->getScripts()) {
				script->init();
			}
		}
	}
}

void Application::fixedUpdate()
{
}

void Application::update()
{
	if (auto componentList = _componentManager.getComponentList<ScriptLoader>()) {
		for (auto componentPair : *componentList) {
			ScriptLoader* scriptLoader = componentPair.second.get();
			for (auto script : scriptLoader->getScripts()) {
				script->update();
			}
		}
	}

	for (auto rootObj : _current_scene->getRootObjects()) {
		iterateTransform(rootObj);
	}

	if (auto componentList = _componentManager.getComponentList<Camera>()) {
		for (auto componentPair : *componentList) {
			Camera* camera = componentPair.second.get();
			camera->update(_window_size, shader);
		}
	}
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader.getProgram());

	Camera* mainCamera = Camera::main;
	if (!mainCamera) {
		std::cout << "No Camera." << std::endl;
		return;
	}
	glUniformMatrix4fv(shader.getUniformLocation("view_matrix"), 1, GL_TRUE, mainCamera->view_matrix);
	glUniformMatrix4fv(shader.getUniformLocation("projection_matrix"), 1, GL_TRUE, mainCamera->projection_matrix);

	if (auto componentList = _componentManager.getComponentList<Light>()) {
		for (auto componentPair : *componentList) {
			Light* light = componentPair.second.get();
			light->render(shader);
		}
	}

	if (auto componentList = _componentManager.getComponentList<Renderer>()) {
		for (auto componentPair : *componentList) {
			Renderer* renderer = componentPair.second.get();
			renderer->render(shader);
		}
	}

	glfwSwapBuffers(_window);
}

void Application::terminate()
{
	cg_destroy_window(_window);
}

void Application::reshape(GLFWwindow* window, int width, int height)
{
	_window_size = ivec2(width, height);
	glViewport(0, 0, width, height);
}

void Application::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::processKeyEvent(key, scancode, action, mods);
}

void Application::mouse(GLFWwindow* window, int button, int action, int mods)
{
	Input::processMouseClickEvent(button, action, mods);
}

void Application::motion(GLFWwindow* window, double x, double y)
{
	dvec2 pos;
	glfwGetCursorPos(window, &pos.x, &pos.y);
	Input::processMouseMoveEvent(pos, _window_size);
}