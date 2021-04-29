#pragma once

#include <cmath>

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
