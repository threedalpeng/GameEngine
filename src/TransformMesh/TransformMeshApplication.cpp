#include "engine/Core.h"
#include "TransformScene.h"

class TransformApplication : public Application {
public:
	TransformApplication(const char* title, ivec2 window_size) : Application(title, window_size) {};
private:
	void init() {
		/* Application init */
		Application::init();

		/* GL Setting Herer*/
		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f); // set clear color
		glEnable(GL_CULL_FACE);									   // turn on backface culling
		glEnable(GL_DEPTH_TEST);								   // turn on depth tests

		/* Scene loading */
		Scene* scene = new TransformScene();
		loadScene(scene);
	}

	void update() {
		Application::update();
		if (Input::getKeyDown(GLFW_KEY_ESCAPE)) {
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