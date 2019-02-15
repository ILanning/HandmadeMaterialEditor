#ifndef HANDMADE_VECTOR2_H
#define HANDMADE_VECTOR2_H

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

	///Gives the vector a magnitude of 1 without changing its direction.
	Vector2 Normalize() const;
	///Calculates the dot product of the two vectors.
	real32 Dot(const Vector2 &b) const;
	///Returns the length of the vector.
	real32 Magnitude() const;
	///Returns the squared length of the vector.
	real32 MagnitudeSquared() const;
	///Returns true if the vector is within certain distance of another.
	bool CloseTo(const Vector2 &other, real32 margin) const;

	///Interpolates between two vectors at a linear rate.
	///@param start The vector to move from
	///@param end The vector to move to
	///@param fraction How far along the line between the two vectors the result will be, with 0 being at start and 1 being at end.
	static Vector2 Lerp(const Vector2 &start, const Vector2 &end, const real32 &fraction);
	///Interpolates between two vectors at a sine-like rate, accelerating at first and decelerating towards the end.
	///@param start The vector to move from
	///@param end The vector to move to
	///@param fraction How close the motion should be to completion, with 0 being at start and 1 being at end.
	static Vector2 CosInterpolate(const Vector2 &start, const Vector2 &end, const real32 &fraction);

	//TODO:  Implement all operators' reversed counterparts
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

#endif //HANDMADE_VECTOR2_H