#pragma once

#include "cg/cgmath.h"

/*
namespace engine {
	template <typename T>
	class Vector2 {
	public:
		Vector2();
		Vector2(const T x, const T y);

		T x;
		T y;

		inline T magnitude() const;
		inline T magnitude2() const;
		inline T dot(const Vector2<T>& vector) const;
		inline Vector2<T> normalize() const;

		inline Vector2<T>& operator=(const T a);

		inline Vector2<T>& operator+=(const T a);
		inline Vector2<T>& operator-=(const T a);
		inline Vector2<T>& operator*=(const T a);
		inline Vector2<T>& operator/=(const T a);
		inline Vector2<T>& operator+=(const Vector2<T>& a);
		inline Vector2<T>& operator-=(const Vector2<T>& a);
		inline Vector2<T>& operator*=(const Vector2<T>& a);
		inline Vector2<T>& operator/=(const Vector2<T>& a);

		inline Vector2<T> operator+(const T a) const;
		inline Vector2<T> operator-(const T a) const;
		inline Vector2<T> operator*(const T a) const;
		inline Vector2<T> operator/(const T a) const;
		inline Vector2<T> operator+(const Vector2<T>& a) const;
		inline Vector2<T> operator-(const Vector2<T>& a) const;
		inline Vector2<T> operator*(const Vector2<T>& a) const;
		inline Vector2<T> operator/(const Vector2<T>& a) const;
	};
}
*/

class Quaternion {
public:
	float x, y, z, w;
	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Quaternion(vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	Quaternion(vec4 v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	inline Quaternion operator*(const float n) const {
		return Quaternion(x * n, y * n, z * n, w * n);
	}
	inline Quaternion operator/(const float n) const {
		return Quaternion(x / n, y / n, z / n, w / n);
	}
	inline Quaternion operator*(const Quaternion& q) const {
		vec3 v = vec3(x, y, z);
		vec3 qV = vec3(q.x, q.y, q.z);
		vec3 tV = w * qV + q.w * v + v.cross(qV);
		float tW = w * q.w - v.dot(qV);
		return Quaternion(tV, tW).normalize();
	}
	inline vec3 operator*(const vec3& v) const {
		return (mat3(this->toMatrix()) * v);
	}

	inline float length() const {
		return sqrtf(x * x + y * y + z * z + w * w);
	}
	inline float length2() const {
		return x * x + y * y + z * z + w * w;
	}
	inline Quaternion normalize() const {
		return Quaternion(x, y, z, w) / length();
	}

	std::pair<vec3, float> toAxisAngle() {
		vec3 axis;
		float angle = 2.f * acosf(w);
		float s = sqrtf(1 - w * w);
		if (s < 0.00001f) {
			axis.x = x;
			axis.y = y;
			axis.z = z;
		}
		else {
			axis.x = x / s;
			axis.y = y / s;
			axis.z = z / s;
		}
		angle = angle * 180.f / PI;
		return std::make_pair(axis, angle);
	}

	mat4 toMatrix() const {
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;
		float w2 = w * w;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float xw = x * w;
		float yw = y * w;
		float zw = z * w;
		return mat4{
			1.f - 2.f * (y2 + z2), 2.f * (xy - zw), 2.f * (xz + yw), 0.f,
			2.f * (xy + zw), 1.f - 2.f * (x2 + z2), 2.f * (yz - xw), 0.f,
			2.f * (xz - yw), 2.f * (yz + xw), 1.f - 2.f * (x2 + y2), 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	static inline Quaternion axisAngle(vec3 axis, float angle) {
		axis = axis.normalize();
		angle = angle * PI / 180.f;
		float s = sinf(angle / 2.f);
		Quaternion q;
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = cosf(angle / 2.f);
		return q.normalize();
	}
};