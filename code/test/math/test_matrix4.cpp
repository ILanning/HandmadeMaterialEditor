#ifndef HANDMADE_TEST_MATRIX4
#define HANDMADE_TEST_MATRIX4

#include "../../math/Matrix4.h"

TEST_CASE("Testing the Matrix4 class")
{
	CHECK(Matrix4::ElementCount == 16);
	Matrix4 a = { 1, 0,  -4, 7,
				  2, 6.5, 8, 5,
				  9, 2,   6, 2.5,
				  0, 1,   6, 3 };
	Matrix4 b = { 1, 5,  4, 3,
				  3, 2,  4, -1,
				  5, 2, -3, -2,
				  5, 6, 7, 8 };

	SUBCASE("Testing Addition")
	{
		Matrix4 c = { 2, 5, 0, 10,   5, 8.5, 12, 4,    14, 4, 3, 0.5,   5, 7, 13, 11 };
		CHECK(a + b == c);

		Matrix4 d = { 4, 7, 2, 12,   7, 10.5, 14, 6,   16, 6, 5, 2.5,   7, 9, 15, 13 };
		CHECK(c + 2 == d);
	}
	SUBCASE("Testing Subtraction")
	{
		Matrix4 c = { 0, -5, -8, 4,   -1, 4.5, 4, 6,    4, 0, 9, 4.5,   -5, -5, -1, -5 };
		CHECK(a - b == c);

		Matrix4 d = { -2, -7, -10, 2,  -3, 2.5, 2, 4,   2, -2, 7, 2.5,   -7, -7, -3, -7 };
		CHECK(c - 2 == d);
	}
	SUBCASE("Testing Multiplication")
	{
		//Matrix4 c = { -19, -3, 16,   61.5, 39, 10,    45, 61, 26 };
		//CHECK(a * b == c);

		Matrix4 d = { 2, 0, -8, 14,   4, 13, 16, 10,    18, 4, 12, 5,   0, 2, 12, 6 };
		CHECK(a * 2 == d);
	}
	SUBCASE("Testing Division")
	{
		Matrix4 c = { 0.5, 0, -2, 3.5,   1, 3.25, 4, 2.5,    4.5, 1, 3, 1.25,   0, 0.5, 3, 1.5 };
		CHECK(a / 2 == c);
	}
	SUBCASE("Testing Transpose")
	{
		Matrix4 c = { 1, 2, 3, 4,    5, 6, 7, 8,    9, 10, 11, 12,    13, 14, 15, 16 };
		Matrix4 d = { 1, 5, 9, 13,   2, 6, 10, 14,  3, 7, 11, 15,     4, 8, 12, 16 };
		CHECK(c.Transpose() == d);
	}
	SUBCASE("Testing Transform")
	{
		Matrix4 c = Matrix4::CreateScale(2, 3, 4);
		Vector3 vec = { 4, 7, 3.5 };
		Vector3 result = { 8, 21, 14 };
		CHECK(c.Transform(vec) == result);
	}
}
/*
*/

#endif