#pragma once
#include "cg/cgmath.h"
#include "engine/Object/Object.h"

class Camera : Object
{
public:
	Camera() : Object() {};

	vec3 eye = vec3(0, 30, 300);
	vec3 at = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);
	mat4 view_matrix = mat4::look_at(eye, at, up);

	float fovy = PI / 4.0f; // must be in radian
	float aspect_ratio = 1.f;
	float dNear = 1.0f;
	float dFar = 1000.0f;
	mat4 projection_matrix;
};
