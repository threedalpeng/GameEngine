#pragma once
#include "engine/Component/Component.h"
#include "cg/cgmath.h"

class Transform : public Component
{
public:
	Transform(std::shared_ptr<GameObject> obj);

	vec3 position = vec3();
	vec4 rotation = vec4(0.f, 0.f, 0.f, 1.f);
	vec3 scale = vec3(1.f, 1.f, 1.f);
	vec3 worldPosition = vec3();
	vec4 worldRotation = vec4(0.f, 0.f, 0.f, 1.f);
	vec3 worldScale = vec3(1.f, 1.f, 1.f);
	vec3 _normal = vec3();

	void translate(const vec3& v);
	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);

	void update();
	void calculateModelMatrix();
	void calculateModelMatrix(mat4 parentMatrix);
	mat4 getModelMatrix();

	vec3 localToWorldPos(vec3 v);
	// void lookAt(Transform* target, vec3 up = vec3(0, 1, 0));

	static std::pair<vec3, float> eulerToAxisAngle(vec3 euler);
	static vec3 axisAngleToEuler(vec3 axis, float angle);

	static vec4 eulerToQuaternion(vec3 euler);
	static vec3 quaternionToEuler(vec4 quaternion);

	static std::pair<vec3, float> quaternionToAxisAngle(vec4 quaternion);
	static vec4 axisAngleToQuaternion(vec3 axis, float angle);
private:
	mat4 _modelMatrix = mat4();
};
