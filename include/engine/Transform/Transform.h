#pragma once
#include "engine/Component/Component.h"
#include "cg/cgmath.h"

class Transform : public Component
{
public:
	Transform(std::shared_ptr<GameObject> obj);

	vec3 position = vec3();
	vec3 rotation = vec3();
	vec3 scale = vec3(1.f, 1.f, 1.f);
	vec3 _normal = vec3();

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);

	void calculateModelMatrix();
	mat4 getModelMatrix();

	void lookAt(const Transform& transform) {
	}
private:
	mat4 _modelMatrix;
};
