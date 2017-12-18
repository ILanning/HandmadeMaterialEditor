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

	Vector3 Normalize() const;
	real32 Dot(const Vector3 &b) const;
	Vector3 Cross(const Vector3 &b) const;
	real32 Magnitude() const;
	real32 MagnitudeSquared() const;
	bool CloseTo(const Vector3 &other, real32 margin) const;

	static Vector3 Lerp(const Vector3 &start, const Vector3 &end, real32 fraction);
	static Vector3 CosInterpolate(const Vector3 &start, const Vector3 &end, real32 fraction);

	friend Vector3 operator-(Vector3 a);
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

	static Vector3 Up() { return{ 0, 1, 0 }; }
	static Vector3 Down() { return{ 0, -1, 0 }; }
	static Vector3 Left() { return{ -1, 0, 0 }; }
	static Vector3 Right() { return{ 1, 0, 0 }; }
	static Vector3 Forward() { return{ 0, 0, 1 }; }
	static Vector3 Back() { return{ 0, 0, -1 }; }

	static Vector3 Zero() { return{ 0, 0, 0 }; }
	static Vector3 One() { return{ 1, 1, 1 }; }
};

#include "Vector3.cpp"

#endif