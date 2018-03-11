#ifndef HANDMADE_MATRIX3_CPP
#define HANDMADE_MATRIX3_CPP

#include "Matrix3.h"

Matrix3 Matrix3::Inverse()
{
	//One over determinant, times transpose of cofactor
	return this->Cofactor().Transpose() / this->Determinant();
}

real32 TwoByTwoDeterminant(real32 a, real32 b, real32 c, real32 d)
{
	return a * d - b * c;
}

Matrix3 Matrix3::Cofactor()
{
	const Matrix3 a = *this;
	Matrix3 result;
	result.m00 =  TwoByTwoDeterminant(a.m11, a.m21, a.m12, a.m22);
	result.m10 = -TwoByTwoDeterminant(a.m01, a.m21, a.m02, a.m22);
	result.m20 =  TwoByTwoDeterminant(a.m01, a.m11, a.m02, a.m12);

	result.m01 = -TwoByTwoDeterminant(a.m10, a.m20, a.m12, a.m22);
	result.m11 =  TwoByTwoDeterminant(a.m00, a.m20, a.m02, a.m22);
	result.m21 = -TwoByTwoDeterminant(a.m00, a.m10, a.m02, a.m12);

	result.m02 =  TwoByTwoDeterminant(a.m10, a.m20, a.m11, a.m21);
	result.m12 = -TwoByTwoDeterminant(a.m00, a.m20, a.m01, a.m21);
	result.m22 =  TwoByTwoDeterminant(a.m00, a.m10, a.m01, a.m11);
	/*	+ - +
		- + -
		+ - +	
	*/
	return result;
}

real32 Matrix3::Determinant()
{
	const Matrix3 a = *this;
	real32 result = 0;
	result += a.m00 * a.m11 * a.m22;
	result += a.m10 * a.m21 * a.m02;
	result += a.m20 * a.m01 * a.m12;
	result -= a.m20 * a.m11 * a.m02;
	result -= a.m10 * a.m01 * a.m22;
	result -= a.m00 * a.m21 * a.m12;
	return result;
}

Vector2 Matrix3::Transform(const Vector2 &a)
{
	Vector2 result = {};
	Vector3 base = { a.x, a.y, 1 };

	int32 size = Matrix3::RowCount;
	for (int32 yPos = 0; yPos < size; yPos++)
	{
		real32 cellResult = 0;
		for (int32 i = 0; i < size; i++)
		{
			cellResult += base.elements[i] * this->m[i][yPos];
		}
		result.elements[yPos] = cellResult;
	}

	return result;
}

Matrix3 Matrix3::Transpose()
{
	Matrix3 result;
	for (int32 x = 0; x < Matrix3::RowCount; x++)
	{
		for (int32 y = 0; y < Matrix3::RowCount; y++)
		{
			result.m[x][y] = this->m[y][x];
		}
	}
	return result;
}

Matrix3 operator+(const Matrix3 &a, const Matrix3 &b)
{
	Matrix3 result;
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] + b.elements[i];
	}
	return result;
}

Matrix3 operator+(const Matrix3 &a, const real32 &b)
{
	Matrix3 result;
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] + b;
	}
	return result;
}

Matrix3 operator-(const Matrix3 &a, const Matrix3 &b)
{
	Matrix3 result;
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] - b.elements[i];
	}
	return result;
}

Matrix3 operator-(const Matrix3 &a, const real32 &b)
{
	Matrix3 result;
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] - b;
	}
	return result;
}

//TODO(Ian): Check to see whether or not this is any slower than a more hardcoded implementation
Matrix3 operator*(const Matrix3 &a, const Matrix3 &b)
{
	Matrix3 result = {};

	int32 size = Matrix3::RowCount;
	for (int32 yPos = 0; yPos < size; yPos++)
	{
		for (int32 xPos = 0; xPos < size; xPos++)
		{
			real32 cellResult = 0;
			for (int32 i = 0; i < size; i++)
			{
				cellResult += a.m[xPos][i] * b.m[i][yPos];
			}
			result.m[xPos][yPos] = cellResult;
		}
	}

	return result;
}


Matrix3 operator*(const Matrix3 &a, const real32 &b)
{
	Matrix3 result = {};
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] * b;
	}
	return result;
}

//TODO(Ian): Should this throw when b == 0?
Matrix3 operator/(const Matrix3 &a, const real32 &b)
{
	Matrix3 result = {};
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] / b;
	}
	return result;
}

Matrix3 &operator+=(Matrix3 &a, const Matrix3 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		a.elements[i] += b.elements[i];
	}
	return a;
}

Matrix3 &operator+=(Matrix3 &a, const real32 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		a.elements[i] += b;
	}
	return a;
}

Matrix3 &operator-=(Matrix3 &a, const Matrix3 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		a.elements[i] -= b.elements[i];
	}
	return a;
}

Matrix3 &operator-=(Matrix3 &a, const real32 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		a.elements[i] -= b;
	}
	return a;
}

Matrix3 &operator*=(Matrix3 &a, const Matrix3 &b)
{
	a = a * b;
	return a;	
}

Matrix3 &operator*=(Matrix3 &a, const real32 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		a.elements[i] *= b;
	}
	return a;
}

Matrix3 &operator/=(Matrix3 &a, const real32 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		a.elements[i] /= b;
	}
	return a;
}

bool operator==(const Matrix3 &a, const Matrix3 &b)
{
	for (int32 i = 0; i < Matrix3::ElementCount; i++)
	{
		if (a.elements[i] != b.elements[i])
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const Matrix3 &a, const Matrix3 &b)
{
	return !(a == b);
}

#endif //HANDMADE_MATRIX3_CPP