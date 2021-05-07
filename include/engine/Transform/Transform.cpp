#include "Transform.h"
#include <iostream>

vec4 quaternionMultiply(vec4 o, vec4 q) {
	vec3 rVec = vec3(o.x, o.y, o.z);
	vec3 qVec = vec3(q.x, q.y, q.z);
	vec3 tVec = o.w * qVec + q.w * rVec + rVec.cross(qVec);
	float tW = o.w * q.w - rVec.dot(qVec);
	return vec4(tVec, tW).normalize();
}

Transform::Transform(std::shared_ptr<GameObject> obj) : Component(obj) {}

void Transform::translate(const vec3& v) {
	position += v;
}
void Transform::translate(float x, float y, float z) {
	vec3 v = vec3(x, y, z);
	position += v;
}

void Transform::rotate(float x, float y, float z) {
	vec4 q = eulerToQuaternion(vec3(x, y, z));
	rotation = quaternionMultiply(rotation, q);
}

void Transform::update() {
	GameObject* parent = getCurrentObject()->getParent();
	if (parent) {
		Transform* parentTransform = parent->getComponent<Transform>();
		calculateModelMatrix(parentTransform->getModelMatrix());
		worldPosition = vec3(_modelMatrix._14, _modelMatrix._24, _modelMatrix._34);
		worldRotation = quaternionMultiply(parentTransform->worldRotation, worldRotation);
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
	auto rotateAxisAngle = quaternionToAxisAngle(rotation);
	_modelMatrix = mat4::translate(position)
		* mat4::rotate(rotateAxisAngle.first, rotateAxisAngle.second * PI / 180.f)
		* mat4::scale(scale);
}
void Transform::calculateModelMatrix(mat4 parentMatrix) {
	auto rotateAxisAngle = quaternionToAxisAngle(rotation);
	_modelMatrix = mat4::translate(position)
		* mat4::rotate(rotateAxisAngle.first, rotateAxisAngle.second * PI / 180.f)
		* mat4::scale(scale);
	_modelMatrix = parentMatrix * _modelMatrix;
}

mat4 Transform::getModelMatrix() {
	return _modelMatrix;
}

vec3 Transform::localToWorldPos(vec3 v)
{
	vec4 result = _modelMatrix * vec4(v, 1.f);
	return vec3(result.x, result.y, result.z);
}

/*
void Transform::lookAt(Transform* target, vec3 up) {
	vec3 front = vec3(0.f, 0.f, 1.f);
	vec3 n1 = (localToWorldPos(front) - worldPosition).normalize();
	vec3 n2 = (target->worldPosition - worldPosition).normalize();
	float d = n1.dot(n2);
	vec3 axis;
	vec4 p;
	if (d > 0.9998) {
		axis = localToWorldPos(vec3(0.f, 1.f, 0.f) - worldPosition).normalize();
		p = vec4(0.f, 0.f, 0.f, 1.f);
	}
	else {
		if (d < -0.9998) {
			rotate(30.f, 0.f, 0.f);
			n1 = (localToWorldPos(front) - worldPosition).normalize();
		}

		axis = n1.cross(n2);
		p = vec4(axis, 1.f + d).normalize();
	}
	quaternionToAxisAngle(p);
	quaternionToEuler(p);

	/*
	mat3 matrix = {
		n2.y * n2.y + n2.z * n2.z, -n2.x * n2.y,-n2.x * n2.z,
		-n2.y * n2.x, n2.x * n2.x + n2.z + n2.z, -n2.y * n2.z,
		-n2.z * n2.x, -n2.z * n2.y, n2.x * n2.x + n2.y * n2.y
	};

	vec3 pUp = (matrix * up).normalize();
	vec3 pY = (matrix * axis).normalize();
	d = pUp.dot(pY);
	vec4 twist;
	if (d > 0.9998) {
		twist = vec4(0.f, 0.f, 0.f, 1.f);
	}
	else {
		if (d < -0.9998) {
			twist = axisAngleToQuaternion(n2, 180);
		}
		else {
			vec3 pAxis = pY.cross(pUp);
			twist = vec4(pAxis, 1.f + d).normalize();
		}
	}
	quaternionToEuler(p);
	quaternionToEuler(twist);
	quaternionToEuler(quaternionMultiply(p, twist));

	// rotation = quaternionMultiply(rotation, quaternionMultiply(p, twist));
	rotation = quaternionMultiply(rotation, p);
}
*/

std::pair<vec3, float> Transform::eulerToAxisAngle(vec3 euler) {
	float radx = euler.x * PI / 180.f;
	float rady = euler.y * PI / 180.f;
	float radz = euler.z * PI / 180.f;

	float c1 = cosf(radx / 2.f);
	float c2 = cosf(rady / 2.f);
	float c3 = cosf(radz / 2.f);
	float s1 = sinf(radx / 2.f);
	float s2 = sinf(rady / 2.f);
	float s3 = sinf(radz / 2.f);

	vec3 v;
	v.x = s1 * c2 * c3 + c1 * s2 * s3;
	v.y = c1 * s2 * c3 - s1 * c2 * s3;
	v.z = s1 * s2 * c3 + c1 * c2 * s3;
	float angle = 2 * acosf(c1 * c2 * c3 - s1 * s2 * s3);

	angle = angle * 180.f / PI;

	return std::make_pair(v.normalize(), angle);
}
vec3 Transform::axisAngleToEuler(vec3 axis, float angle) {
	angle = angle * PI / 180.f;
	float s = sinf(angle);
	float c = cosf(angle);
	axis = axis.normalize();

	if (-0.0001f < angle && angle < 0.0001f) {
		return vec3();
	}

	vec3 euler;
	if ((axis.x * axis.y * (1 - c) + axis.z * s) > 0.998f) {
		euler.x = 0.f;
		euler.y = 2.f * atan2f(axis.x * sinf(angle / 2.f), cosf(angle / 2.f));
		euler.z = PI / 2.f;
	}
	else if ((axis.x * axis.y * (1 - c) + axis.z * s) < -0.998f) {
		euler.x = 0.f;
		euler.y = -2.f * atan2f(axis.x * sinf(angle / 2.f), cosf(angle / 2.f));
		euler.z = -PI / 2.f;
	}
	else {
		euler.x = atan2f(
			axis.x * s - axis.y * axis.z * (1 - c),
			1 - (axis.x * axis.x + axis.z * axis.z) * (1 - c));
		euler.y = atan2f(
			axis.y * s - axis.x * axis.z * (1 - c),
			1 - (axis.y * axis.y + axis.z * axis.z) * (1 - c));
		euler.z = asinf(axis.x * axis.y * (1 - c) + axis.z * s);
	}

	euler.x = euler.x * 180.f / PI;
	euler.y = euler.y * 180.f / PI;
	euler.z = euler.z * 180.f / PI;

	return euler;
}
vec4 Transform::eulerToQuaternion(vec3 euler) {
	float radx = euler.x * PI / 180.f;
	float rady = euler.y * PI / 180.f;
	float radz = euler.z * PI / 180.f;

	float c1 = cosf(radx / 2.f);
	float c2 = cosf(rady / 2.f);
	float c3 = cosf(radz / 2.f);
	float s1 = sinf(radx / 2.f);
	float s2 = sinf(rady / 2.f);
	float s3 = sinf(radz / 2.f);

	vec4 q;
	q.w = c1 * c2 * c3 - s1 * s2 * s3;
	q.x = s1 * c2 * c3 + c1 * s2 * s3;
	q.y = c1 * s2 * c3 - s1 * c2 * s3;
	q.z = s1 * s2 * c3 + c1 * c2 * s3;

	return q;
}
vec3 Transform::quaternionToEuler(vec4 q) {
	vec3 euler;

	q = q.normalize();

	float x2 = q.x * q.x;
	float y2 = q.y * q.y;
	float z2 = q.z * q.z;
	float w2 = q.w * q.w;

	float unit = x2 + y2 + z2 + w2;
	float test = q.x * q.y + q.z * q.w;
	if (test > 0.499 * unit) {
		euler.x = 0.f;
		euler.y = 2.f * atan2f(q.x, q.w);
		euler.z = PI / 2.f;
	}
	else if (test < -0.499 * unit) {
		euler.x = 0.f;
		euler.y = -2.f * atan2f(q.x, q.w);
		euler.z = -PI / 2.f;
	}
	else {
		euler.x = atan2f(
			2.f * q.x * q.w - 2.f * q.y * q.z,
			-x2 + y2 - z2 + w2);
		euler.y = atan2f(
			2.f * q.y * q.w - 2.f * q.x * q.z,
			x2 - y2 - z2 + w2);
		euler.z = asinf(2.f * test / unit);
	}

	euler.x = euler.x * 180.f / PI;
	euler.y = euler.y * 180.f / PI;
	euler.z = euler.z * 180.f / PI;

	return euler;
}

std::pair<vec3, float> Transform::quaternionToAxisAngle(vec4 q) {
	if (q.w > 1)
		q = q.normalize();
	vec3 axis;
	float angle = 2.f * acosf(q.w);
	float s = sqrtf(1 - q.w * q.w);
	if (s < 0.00001f) {
		axis.x = q.x;
		axis.y = q.y;
		axis.z = q.z;
	}
	else {
		axis.x = q.x / s;
		axis.y = q.y / s;
		axis.z = q.z / s;
	}
	angle = angle * 180.f / PI;
	return std::make_pair(axis, angle);
}

vec4 Transform::axisAngleToQuaternion(vec3 axis, float angle) {
	axis = axis.normalize();
	angle = angle * PI / 180.f;
	float s = sinf(angle / 2.f);
	vec4 q;
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	q.w = cosf(angle / 2.f);
	return q;
}