#ifndef HANDMADE_MATRIX3
#define HANDMADE_MATRIX3

#include <math.h>
#include "..\handmade_typedefs.h"
#include "Vector2.h"

struct Matrix3
{
	const static int32 RowCount = 3;
	const static int32 ElementCount = RowCount * RowCount;
	union
	{
		struct
		{
			real32 m00;
			real32 m01;
			real32 m02;
			real32 m10;
			real32 m11;
			real32 m12;
			real32 m20;
			real32 m21;
			real32 m22;
		};
		real32 m[3][3];
		real32 elements[9];
	};

	Matrix3 Inverse();
	real32 Determinant();
	Matrix3 Cofactor();
	Matrix3 Transpose();
	Vector2 Transform(const Vector2 &a);

	static Matrix3 Identity()
	{		
		Matrix3 result = { 1, 0, 0,
						   0, 1, 0,
						   0, 0, 1};
		return result;
	}

	static Matrix3 CreateRotation(const float &radians)
	{
		Matrix3 result = { cosf(radians), -sinf(radians), 0,
			sinf(radians),  cosf(radians), 0,
			0,              0,				1 };
		return result;
	}

	static Matrix3 CreateScale(const Vector2 &scaleFactor)
	{
		Matrix3 result = { scaleFactor.x, 0, 0,
			0, scaleFactor.y, 0,
			0,				0, 1 };
		return result;
	}

	static Matrix3 CreateScale(const real32 &x, const real32 &y)
	{
		Matrix3 result = { x, 0, 0,
						   0, y, 0,
						   0, 0, 1 };
		return result;
	}

	static Matrix3 CreateTranslation(const Vector2 &position)
	{
		Matrix3 result = { 1, 0, position.x,
			0, 1, position.y,
			0, 0, 1 };
		return result;
	}

	friend Matrix3 operator+(const Matrix3 &a, const Matrix3 &b);
	friend Matrix3 operator+(const Matrix3 &a, const real32 &b);
	friend Matrix3 operator-(const Matrix3 &a, const Matrix3 &b);
	friend Matrix3 operator-(const Matrix3 &a, const real32 &b);
	friend Matrix3 operator*(const Matrix3 &a, const Matrix3 &b);
	friend Matrix3 operator*(const Matrix3 &a, const real32 &b);
	friend Matrix3 operator/(const Matrix3 &a, const real32 &b);

	friend Matrix3 &operator+=(const Matrix3 &a, const Matrix3 &b);
	friend Matrix3 &operator+=(const Matrix3 &a, const real32 &b);
	friend Matrix3 &operator-=(const Matrix3 &a, const Matrix3 &b);
	friend Matrix3 &operator-=(const Matrix3 &a, const real32 &b);
	friend Matrix3 &operator*=(const Matrix3 &a, const real32 &b);
	friend Matrix3 &operator/=(const Matrix3 &a, const real32 &b);

	friend bool operator==(const Matrix3 &a, const Matrix3 &b);
	friend bool operator!=(const Matrix3 &a, const Matrix3 &b);
};

#include "Matrix3.cpp"

#endif