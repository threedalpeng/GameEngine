#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> obj) : Component(obj) {}

void Transform::translate(float x, float y, float z)
{
}

void Transform::rotate(float x, float y, float z)
{
}

mat4 Transform::getModelMatrix()
{
	return mat4::translate(position) * mat4::scale(scale);
}