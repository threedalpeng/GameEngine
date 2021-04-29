#ifndef __PHYSICS_CORE_H__
#define __PHYSICS_CORE_H__

namespace physics {
	class Vec3 {
	public:
		float x;
		float y;
		float z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	};
}

#endif __PHYSICS_CORE_H__