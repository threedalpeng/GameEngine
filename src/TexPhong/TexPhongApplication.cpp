#include "engine/Core.h"
#include "TexPhongScene.h"

class TexPhongApplication : public Application {
public:
	TexPhongApplication(const char* title, ivec2 window_size) : Application(title, window_size) {};
private:
	void init() {
		Application::init();

		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
		glEnable(GL_CULL_FACE);										// turn on backface culling
		glEnable(GL_DEPTH_TEST);									// turn on depth tests
		glEnable(GL_TEXTURE_2D);									// enable texturing
		glActiveTexture(GL_TEXTURE0);								// notify GL the current texture slot is 0

		Scene* scene = new TexPhongScene();
		loadScene(scene);
	}

	void update() {
		Application::update();
		if (Input::getKeyDown(GLFW_KEY_ESCAPE) || Input::getKeyDown(GLFW_KEY_Q)) {
			glfwSetWindowShouldClose(_window, GL_TRUE);
		}
	}

	void fixedUpdate() {
		Application::fixedUpdate();
	}
};

int main() {
	TexPhongApplication* app = new TexPhongApplication("texphong", cg_default_window_size());
	app->run();
}