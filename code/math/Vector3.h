#ifndef HANDMADE_VECTOR3_H
#define HANDMADE_VECTOR3_H

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

	///Gives the vector a magnitude of 1 without changing its direction.
	Vector3 Normalize() const;
	///Calculates the dot product of the two vectors.
	real32 Dot(const Vector3 &b) const;
	///Calculates the cross product of the two vectors.
	Vector3 Cross(const Vector3 &b) const;
	///Returns the length of the vector.
	real32 Magnitude() const;
	///Returns the squared length of the vector.
	real32 MagnitudeSquared() const;
	///Returns true if the vector is within certain distance of another.
	bool CloseTo(const Vector3 &other, real32 margin) const;

	///Interpolates between two vectors at a linear rate.
	///@param start The vector to move from
	///@param end The vector to move to
	///@param fraction How far along the line between the two vectors the result will be, with 0 being at start and 1 being at end.
	static Vector3 Lerp(const Vector3 &start, const Vector3 &end, real32 fraction);
	///Interpolates between two vectors at a sine-like rate, accelerating at first and decelerating towards the end.
	///@param start The vector to move from
	///@param end The vector to move to
	///@param fraction How close the motion should be to completion, with 0 being at start and 1 being at end.
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

	///+1 on Y axis.
	static Vector3 Up() { return{ 0, 1, 0 }; }
	///-1 on Y axis.
	static Vector3 Down() { return{ 0, -1, 0 }; }
	///-1 on X axis.
	static Vector3 Left() { return{ -1, 0, 0 }; }
	///+1 on X axis.
	static Vector3 Right() { return{ 1, 0, 0 }; }
	///+1 on Z axis.
	static Vector3 Forward() { return{ 0, 0, 1 }; }
	///-1 on Z axis.
	static Vector3 Back() { return{ 0, 0, -1 }; }

	static Vector3 Zero() { return{ 0, 0, 0 }; }
	static Vector3 One() { return{ 1, 1, 1 }; }
};

#endif //HANDMADE_VECTOR3_H