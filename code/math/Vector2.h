#ifndef HANDMADE_VECTOR2
#define HANDMADE_VECTOR2

#include <math.h>
#include "..\handmade_typedefs.h"

struct Vector2
{
	static const int ElementCount = 2;

	union
	{
		struct
		{
			real32 x, y;
		};
		struct
		{
			real32 u, v;
		};
		real32 elements[ElementCount];
	};

	Vector2 Normalize();
	real32 Dot(const Vector2 &b);
	real32 Magnitude();
	real32 MagnitudeSquared();

	static Vector2 Lerp(const Vector2 &start, const Vector2 &end, const real32 &fraction);
	static Vector2 CosInterpolate(const Vector2 &start, const Vector2 &end, const real32 &fraction);

	//TODO(Ian):  Implement all operators' reversed counterparts
	friend Vector2 operator+(const Vector2 &a, const Vector2 &b);
	friend Vector2 operator+(const Vector2 &a, const real32 &b);
	friend Vector2 operator-(const Vector2 &a, const Vector2 &b);
	friend Vector2 operator-(const Vector2 &a, const real32 &b);
	friend Vector2 operator*(const Vector2 &a, const real32 &b);
	friend Vector2 operator/(const Vector2 &a, const real32 &b);

	friend Vector2& operator+=(Vector2 &a, const Vector2 &b);
	friend Vector2& operator+=(Vector2 &a, const real32 &b);
	friend Vector2& operator-=(Vector2 &a, const Vector2 &b);
	friend Vector2& operator-=(Vector2 &a, const real32 &b);
	friend Vector2& operator*=(Vector2 &a, const real32 &b);
	friend Vector2& operator/=(Vector2 &a, const real32 &b);

	friend bool operator==(const Vector2 &a, const Vector2 &b);
	friend bool operator!=(const Vector2 &a, const Vector2 &b);
};

#include "Vector2.cpp"

#endif