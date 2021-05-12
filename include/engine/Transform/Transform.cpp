#include "Transform.h"
#include <iostream>

Transform::Transform(std::shared_ptr<GameObject> obj) : Component(obj) {}

void Transform::translate(const vec3& v) {
	position += v;
}
void Transform::translate(float x, float y, float z) {
	vec3 v = vec3(x, y, z);
	position += v;
}

void Transform::rotate(vec3 axis, float angle) {
	Quaternion q = Quaternion::axisAngle(axis, angle);
	rotation = rotation * q;
}

void Transform::rotateAround(vec3 worldPoint, vec3 axis, float angle)
{
	Quaternion q = Quaternion::axisAngle(axis, angle);
	position = q * (position - worldPoint) + worldPoint;
	rotation = q * rotation;
}

void Transform::update() {
	GameObject* parent = getCurrentObject()->getParent();
	if (parent) {
		Transform* parentTransform = parent->getComponent<Transform>();
		calculateModelMatrix(parentTransform->getModelMatrix());
		worldPosition = vec3(_modelMatrix._14, _modelMatrix._24, _modelMatrix._34);
		worldRotation = parentTransform->worldRotation * worldRotation;
		worldScale = parentTransform->worldScale * scale;
	}
	else {
		worldPosition = position;
		worldRotation = rotation;
		worldScale = scale;
		calculateModelMatrix();
	}
}
void Transform::calculateModelMatrix() {
	_modelMatrix = mat4::translate(position)
		* rotation.toMatrix()
		* mat4::scale(scale);
}
void Transform::calculateModelMatrix(mat4 parentMatrix) {
	_modelMatrix = mat4::translate(position)
		* rotation.toMatrix()
		* mat4::scale(scale);
	_modelMatrix = parentMatrix * _modelMatrix;
}

mat4 Transform::getModelMatrix() {
	return _modelMatrix;
}

vec3 Transform::localToWorldPoint(vec3 v)
{
	return mat3(_modelMatrix) * v;
}

vec3 Transform::worldToLocalPoint(vec3 v)
{
	return mat3(_modelMatrix).transpose() * v;
}