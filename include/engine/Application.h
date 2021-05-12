#pragma once
#include <vector>
#include "cg/cgut.h"
#include "cg/cgmath.h"
#include "engine/Input.h"
#include "engine/Scene.h"
#include "engine/Component/ComponentManager.h"
#include "engine/Graphics/Shader.h"

class Application {
public:
	Application(std::string title, ivec2 window_size);

	// run application
	virtual void run();

protected:
	std::string _title;

	// window objects
	GLFWwindow* _window = nullptr;
	ivec2		_window_size;

	uint _frame_count = 0;
	float _frame_rate = 100;

	// Shader
	Shader shader;

	// Scene
	Scene* _current_scene = nullptr;
	void loadScene(Scene* scene);

	// Managers
	Input			 _input;
	ComponentManager _componentManager;

	// Application Lifecycle
	virtual void init();
	virtual void fixedUpdate();
	virtual void update();
	virtual void render();
	virtual void terminate();

	// Event Callback Functions
	virtual void reshape(GLFWwindow* window, int width, int height);
	virtual void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void mouse(GLFWwindow* window, int button, int action, int mods);
	virtual void motion(GLFWwindow* window, double x, double y);
};