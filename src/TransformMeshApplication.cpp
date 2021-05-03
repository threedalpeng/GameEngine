#include "engine/Core.h"
#include "TransformScene.h"

class TransformApplication : public Application {
public:
	TransformApplication(const char* title, ivec2 window_size) : Application(title, window_size) {};
private:
	void init() {
		Application::init();
		Scene* scene = new TransformScene();
		loadScene(scene);
	}

	void update() {
		Application::update();
		if (Input::getKeyDown(GLFW_KEY_ESCAPE) || Input::getKeyDown(GLFW_KEY_Q)) {
			glfwSetWindowShouldClose(_window, GL_TRUE);
		}
	}

	void fixedUpdate() {
	}
};

int main() {
	TransformApplication* app = new TransformApplication("dragon", cg_default_window_size());
	app->run();
}