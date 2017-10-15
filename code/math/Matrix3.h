#ifndef HANDMADE_MATRIX3
#define HANDMADE_MATRIX3

struct Matrix3
{	
	real32 M[3][3];

	Matrix3 operator+(const Matrix3 &b);
	Matrix3 operator+(const int32 &b);
	Matrix3 operator-(const Matrix3 &b);
	Matrix3 operator-(const int32 &b);
	Matrix3 operator*(const Matrix3 &b);
	Matrix3 operator*(const int32 &b);
	Matrix3 operator/(const Matrix3 &b);
	Matrix3 operator/(const int32 &b);

};

#endif