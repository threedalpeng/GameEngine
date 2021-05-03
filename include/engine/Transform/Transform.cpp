#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> obj) : Component(obj) {}

void Transform::translate(float x, float y, float z)
{
}

void Transform::rotate(float x, float y, float z)
{
}

void Transform::calculateModelMatrix() {
	GameObject* parent = getCurrentObject()->getParent();
	_modelMatrix = mat4::translate(position)
		* mat4::rotate(vec3(1.f, 0.f, 0.f), rotation.x)
		* mat4::rotate(vec3(0.f, 1.f, 0.f), rotation.y)
		* mat4::rotate(vec3(0.f, 0.f, 1.f), rotation.z)
		* mat4::scale(scale);
	if (parent)
		_modelMatrix = parent->getComponent<Transform>()->getModelMatrix() * _modelMatrix;
}

mat4 Transform::getModelMatrix() {
	return _modelMatrix;
}