#pragma once
#include <iostream>
#include "engine/Core.h"

class CameraScript : public Script
{
public:
	CameraScript() : Script() {}

	GameObject* trackingTarget = nullptr;
	GameObject* trackball = nullptr;
	GameObject* vAxis = nullptr;

	//bool mouseTracking = false;
	//dvec2 prevCursor = dvec2(0, 0);

	void init() override {
		printHelp();
		getComponent<Camera>()->activateTracking(trackingTarget->getComponent<Transform>());
	}

	void update() override {
		if (Input::getMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
			Transform* cameraTransform = getComponent<Transform>();
			Transform* trackballTransform = trackball->getComponent<Transform>();
			mat4 view_matrix = getComponent<Camera>()->view_matrix;
			dvec2 cursorAxis = Input::getCursorAxis() * 100.f;
			if (abs(cursorAxis.x) > 0.0001 || abs(cursorAxis.y) > 0.0001) {
				vec3 p1 = vec3(float(cursorAxis.x), float(-cursorAxis.y), float(sqrt(std::max(0.0, 10000.0 - cursorAxis.length2())))).normalize();
				vec3 n = vec3(0, 0, 1.0f).cross(p1);
				vec3 v = mat3(trackball->getComponent<Transform>()->getModelMatrix()).transpose() * mat3(view_matrix).transpose() * n;
				float angle = float(cursorAxis.length());
				Transform* vAxisTransform = vAxis->getComponent<Transform>();
				vAxisTransform->position = v * 30;
				trackball->getComponent<Transform>()->rotate(v, angle);
			}
		}
	}

	void printHelp() {
		printf("[help]\n");
		printf("- press ESC or 'q' to terminate the program\n");
		printf("- press F1 or 'h' to see help\n");
		printf("\n");
	}
private:
	mat3 createProjectionMatrix(vec3 v) {
		return mat3(
			v.y * v.y + v.z * v.z, -v.x * v.y, -v.x * v.z,
			-v.y * v.x, v.x * v.x + v.z + v.z, -v.y * v.z,
			-v.z * v.x, -v.z * v.y, v.x * v.x + v.y * v.y
		);
	}
};
