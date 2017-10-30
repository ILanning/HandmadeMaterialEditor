#ifndef HANDMADE_QUATERNION
#define HANDMADE_QUATERNION

#include <math.h>
#include "..\handmade_typedefs.h"

struct Quaternion
{
	static const int ElementCount = 4;

	union
	{
		struct
		{
			real32 x, y, z, w;
		};
		real32 elements[ElementCount];
	};

	Quaternion Normalize();
	real32 Magnitude();
	real32 MagnitudeSquared();
	Quaternion Inverse();

	static Quaternion Identity()
	{
		return { 0, 0, 0, 1 };
	}

	static Quaternion Lerp(const Quaternion &start, const Quaternion &end, const real32 &fraction);
	static Quaternion CosInterpolate(const Quaternion &start, const Quaternion &end, const real32 &fraction);

	friend Quaternion operator*(const Quaternion &a, const Quaternion &b);
	friend Quaternion operator*(const Quaternion &a, const real32 &b);
	friend Quaternion operator/(const Quaternion &a, const real32 &b);
	friend Quaternion& operator*=(Quaternion &a, const Quaternion &b);
	friend Quaternion& operator*=(Quaternion &a, const real32 &b);
	friend Quaternion& operator/=(Quaternion &a, const real32 &b);

	friend bool operator==(const Quaternion &a, const Quaternion &b);
	friend bool operator!=(const Quaternion &a, const Quaternion &b);
};

#include "Quaternion.cpp"

#endif