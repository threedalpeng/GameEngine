#pragma once
#include "cg/cgmath.h"
#include "engine/Object/GameObject.h"
#include "engine/Graphics/Shader.h"

class Camera : public Component
{
public:
	Camera(std::shared_ptr<GameObject> obj) : Component(obj) {};

	void update(ivec2 window_size, Shader& shader) {
		aspect_ratio = window_size.x / static_cast<float>(window_size.y);
		projection_matrix = mat4::perspective(fovy, aspect_ratio, dNear, dFar);
	}

	void setThisMainCamera() {
		main = this;
	}

	vec3 eye = vec3(0, 30, 300);
	vec3 at = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);
	mat4 view_matrix = mat4::look_at(eye, at, up);

	float fovy = PI / 4.0f; // must be in radian
	float aspect_ratio = 1.f;
	float dNear = 1.0f;
	float dFar = 1000.0f;
	mat4 projection_matrix;

	static Camera* main;
};
