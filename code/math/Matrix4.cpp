/*Matrix4 Matrix4::Inverse()
{
	//One over determinant, times transpose of cofactor
	return this->Cofactor().Transpose() / this->Determinant();
}

real32 Matrix4::Determinant()
{
	Matrix4 a = (*this);
	real32 cofactorM00 = ThreeByThreeDeterminant(a.m11, a.m12, a.m13,
												 a.m21, a.m22, a.m23,
												 a.m31, a.m32, a.m33);
	real32 cofactorM10 = ThreeByThreeDeterminant(a.m10, a.m12, a.m13,
												 a.m20, a.m22, a.m23,
												 a.m30, a.m32, a.m33);
	real32 cofactorM20 = ThreeByThreeDeterminant(a.m10, a.m11, a.m13,
												 a.m20, a.m21, a.m23,
												 a.m30, a.m31, a.m33);
	real32 cofactorM30 = ThreeByThreeDeterminant(a.m10, a.m11, a.m12,
												 a.m20, a.m21, a.m22,
												 a.m30, a.m31, a.m32);
	return a.m00 * cofactorM00 - a.m10 * cofactorM10 + a.m20 * cofactorM20 - a.m30 * cofactorM30;
}

Matrix4 Matrix4::Cofactor()
{
	const Matrix4 a = *this;
	Matrix4 result;
	result.m00 = TwoByTwoDeterminant(a.m11, a.m21, a.m12, a.m22);
	result.m10 = -TwoByTwoDeterminant(a.m01, a.m21, a.m02, a.m22);
	result.m20 = TwoByTwoDeterminant(a.m01, a.m11, a.m02, a.m12);

	result.m01 = -TwoByTwoDeterminant(a.m10, a.m20, a.m12, a.m22);
	result.m11 = TwoByTwoDeterminant(a.m00, a.m20, a.m02, a.m22);
	result.m21 = -TwoByTwoDeterminant(a.m00, a.m10, a.m02, a.m12);

	result.m02 = TwoByTwoDeterminant(a.m10, a.m20, a.m11, a.m21);
	result.m12 = -TwoByTwoDeterminant(a.m00, a.m20, a.m01, a.m21);
	result.m22 = TwoByTwoDeterminant(a.m00, a.m10, a.m01, a.m11);
	/*	+ - +
	- + -
	+ - +
	*
	return result;
}

real32 ThreeByThreeDeterminant(const real32 &a, const real32 &b, const real32 &c,
							   const real32 &d, const real32 &e, const real32 &f,
							   const real32 &g, const real32 &h, const real32 &i)
{
	real32 result = 0;
	result += a * e * i;
	result += b * f * g;
	result += c * d * h;
	result -= c * e * g;
	result -= b * d * i;
	result -= a * f * h;
	return result;
}*/

Matrix4 Matrix4::Transpose()
{
	Matrix4 result;
	for (int32 x = 0; x < Matrix4::RowCount; x++)
	{
		for (int32 y = 0; y < Matrix4::RowCount; y++)
		{
			result.m[x][y] = m[y][x];
		}
	}
	return result;
}

Vector3 Matrix4::Transform(const Vector3 &a)
{
	Vector3 result = {};
	real32 base [4] = { a.x, a.y, a.z, 1 };

	int32 size = Matrix4::RowCount;
	for (int32 yPos = 0; yPos < size; yPos++)
	{
		real32 cellResult = 0;
		for (int32 i = 0; i < size; i++)
		{
			cellResult += base[i] * m[yPos][i];
		}
		result.elements[yPos] = cellResult;
	}

	return result;
}

void Matrix4::SetTranslation(const Vector3 &a)
{
	m03 = a.x;
	m13 = a.y;
	m23 = a.z;
}

Matrix4 operator+(const Matrix4 &a, const Matrix4 &b)
{
	Matrix4 result;
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] + b.elements[i];
	}
	return result;
}

Matrix4 operator+(const Matrix4 &a, real32 b)
{
	Matrix4 result;
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] + b;
	}
	return result;
}

Matrix4 operator-(const Matrix4 &a, const Matrix4 &b)
{
	Matrix4 result;
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] - b.elements[i];
	}
	return result;
}

Matrix4 operator-(const Matrix4 &a, real32 b)
{
	Matrix4 result;
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] - b;
	}
	return result;
}

//TODO(Ian): Check to see whether or not this is any slower than a more hardcoded implementation
Matrix4 operator*(const Matrix4 &a, const Matrix4 &b)
{
	Matrix4 result = {};

	int32 size = Matrix4::RowCount;
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


Matrix4 operator*(const Matrix4 &a, real32 b)
{
	Matrix4 result = {};
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] * b;
	}
	return result;
}

Matrix4 operator/(const Matrix4 &a, real32 b)
{
	Matrix4 result = {};
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		result.elements[i] = a.elements[i] / b;
	}
	return result;
}

Matrix4 &operator+=(Matrix4 &a, const Matrix4 &b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		a.elements[i] += b.elements[i];
	}
	return a;
}

Matrix4 &operator+=(Matrix4 &a, real32 b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		a.elements[i] += b;
	}
	return a;
}

Matrix4 &operator-=(Matrix4 &a, const Matrix4 &b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		a.elements[i] -= b.elements[i];
	}
	return a;
}

Matrix4 &operator-=(Matrix4 &a, real32 b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		a.elements[i] -= b;
	}
	return a;
}

Matrix4 &operator*=(Matrix4 &a, const Matrix4 &b)
{
	a = a * b;
	return a;
}

Matrix4 &operator*=(Matrix4 &a, real32 b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		a.elements[i] *= b;
	}
	return a;
}

Matrix4 &operator/=(Matrix4 &a, real32 b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		a.elements[i] /= b;
	}
	return a;
}

bool operator==(const Matrix4 &a, const Matrix4 &b)
{
	for (int32 i = 0; i < Matrix4::ElementCount; i++)
	{
		if (a.elements[i] != b.elements[i])
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const Matrix4 &a, const Matrix4 &b)
{
	return !(a == b);
}