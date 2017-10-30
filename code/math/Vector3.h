#ifndef HANDMADE_VECTOR3
#define HANDMADE_VECTOR3

#include <math.h>
#include "..\handmade_typedefs.h"

struct Vector3
{
	static const int ElementCount = 3;

	union
	{
		struct
		{
			real32 x, y, z;
		};
		struct
		{
			real32 u, v, w;
		};
		struct
		{
			real32 r, g, b;
		};
		real32 elements[ElementCount];
	};

	Vector3 Normalize();
	real32 Dot(const Vector3 &b);
	Vector3 Cross(const Vector3 &b);
	real32 Magnitude();
	real32 MagnitudeSquared();

	static Vector3 Lerp(const Vector3 &start, const Vector3 &end, const real32 &fraction);
	static Vector3 CosInterpolate(const Vector3 &start, const Vector3 &end, const real32 &fraction);

	friend Vector3 operator+(const Vector3 &a, const Vector3 &b);
	friend Vector3 operator+(const Vector3 &a, const real32 &b);
	friend Vector3 operator-(const Vector3 &a, const Vector3 &b);
	friend Vector3 operator-(const Vector3 &a, const real32 &b);
	friend Vector3 operator*(const Vector3 &a, const real32 &b);
	friend Vector3 operator/(const Vector3 &a, const real32 &b);

	friend Vector3& operator+=(Vector3 &a, const Vector3 &b);
	friend Vector3& operator+=(Vector3 &a, const real32 &b);
	friend Vector3& operator-=(Vector3 &a, const Vector3 &b);
	friend Vector3& operator-=(Vector3 &a, const real32 &b);
	friend Vector3& operator*=(Vector3 &a, const real32 &b);
	friend Vector3& operator/=(Vector3 &a, const real32 &b);

	friend bool operator==(const Vector3 &a, const Vector3 &b);
	friend bool operator!=(const Vector3 &a, const Vector3 &b);
};

#include "Vector3.cpp"

#endif