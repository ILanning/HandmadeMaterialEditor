#ifndef HANDMADE_VECTOR3_CPP
#define HANDMADE_VECTOR3_CPP

#include "Vector3.h"
#include <math.h>

Vector3 Vector3::Normalize() const
{
	real32 mag = Magnitude();
	if (mag == 0)
	{
		return { 0, 0, 0 };
	}
	return *this / mag;
}

real32 Vector3::Dot(const Vector3 &other) const
{
	return sqrtf(x * other.x + y * other.y + z * other.z);
}

Vector3 Vector3::Cross(const Vector3 &other) const
{
	return{
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x,
	};
}

real32 Vector3::Magnitude() const
{
	return sqrtf(MagnitudeSquared());
}

real32 Vector3::MagnitudeSquared() const
{
	return x * x + y * y + z * z;
}

bool Vector3::CloseTo(const Vector3 &other, real32 margin) const
{
	for (int32 i = 0; i < Vector3::ElementCount; i++)
	{
		real32 diff = elements[i] - other.elements[i];
		diff = diff < 0 ? -diff : diff;

		if (diff > margin)
		{
			return false;
		}
	}

	return true;
}

Vector3 Vector3::Lerp(const Vector3 &start, const Vector3 &end, real32 fraction)
{
	return (end - start) * fraction + start;
}

Vector3 Vector3::CosInterpolate(const Vector3 &start, const Vector3 &end, real32 fraction)
{
	real32 cosFraction = (1 - cosf(fraction * Pi32)) / 2;
	return Lerp(start, end, cosFraction);
}

Vector3 operator-(Vector3 a)
{
	a.x = -a.x;
	a.y = -a.y;
	a.z = -a.z;
	return a;
}

Vector3 operator+(const Vector3 &a, const Vector3 &b)
{
	return{ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 operator+(const Vector3 &a, const real32 &b)
{
	return{ a.x + b, a.y + b, a.z + b };
}

Vector3 operator-(const Vector3 &a, const Vector3 &b)
{
	return{ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 operator-(const Vector3 &a, const real32 &b)
{
	return{ a.x - b, a.y - b, a.z - b };
}

Vector3 operator*(const Vector3 &a, const real32 &b)
{
	return{ a.x * b, a.y * b, a.z * b };
}

Vector3 operator/(const Vector3 &a, const real32 &b)
{
	return { a.x / b, a.y / b, a.z / b };
}

Vector3& operator+=(Vector3 &a, const Vector3 &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3& operator+=(Vector3 &a, const real32 &b)
{
	a.x += b;
	a.y += b;
	a.z += b;
	return a;
}

Vector3& operator-=(Vector3 &a, const Vector3 &b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vector3& operator-=(Vector3 &a, const real32 &b)
{
	a.x -= b;
	a.y -= b;
	a.z -= b;
	return a;
}

Vector3& operator*=(Vector3 &a, const real32 &b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

Vector3& operator/=(Vector3 &a, const real32 &b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return a;
}

bool operator==(const Vector3 &a, const Vector3 &b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator!=(const Vector3 &a, const Vector3 &b)
{
	return !(a == b);
}

#endif // !HANDMADE_VECTOR3_CPP