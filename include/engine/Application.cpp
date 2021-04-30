#include "engine/Application.h"
#include "engine/Transform/Transform.h"

Application::Application(const char* title, ivec2 window_size)
	: _title(title), _window_size(window_size) {}

void Application::run()
{
	init();

	float t, prev_t = (float)glfwGetTime(), duration;
	float lag = 0.f, fixed_time = 1.f / _frame_rate;

	for (_frame_count = 0; !glfwWindowShouldClose(_window); _frame_count++) {
		t = (float)glfwGetTime();
		duration = t - prev_t;
		prev_t = t;
		lag += duration;

		glfwPollEvents();
		Input::poll();

		// fixed time update
		while (lag >= fixed_time) {
			fixedUpdate();
			lag -= fixed_time;
		}

		if (_frame_count % 20 == 0) {
			printf("                           \r");
			printf(" FPS: %f\r", 1 / duration);
		}
		update(duration);
		render();
	}

	terminate();
}

void Application::loadScene(Scene* scene)
{
	_current_scene = scene;

	_componentManager.createComponentList<Transform>();

	// Initiate Managers By Scene
	_graphicsManager.init(scene);

	_current_scene->init();
}

void Application::init()
{
	if (!(_window = cg_create_window(_title, _window_size.x, _window_size.y))) {
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

	ServiceLocator::provide<ComponentManager>(&_componentManager);
}

void Application::fixedUpdate()
{
}

void Application::update(float duration)
{
}

void Application::render()
{
	_graphicsManager.render();
}

void Application::terminate()
{
	cg_destroy_window(_window);
}

void Application::reshape(GLFWwindow* window, int width, int height)
{
}

void Application::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::processKeyEvent(key, scancode, action, mods);
}

void Application::mouse(GLFWwindow* window, int button, int action, int mods)
{
}

void Application::move(GLFWwindow* window, int button, int action, int mods)
{
}