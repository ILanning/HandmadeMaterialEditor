#ifndef HANDMADE_TEST_MATRIX3
#define HANDMADE_TEST_MATRIX3

#include "../../math/Matrix3.cpp"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../libraries/doctest.h"
#endif

TEST_CASE("Testing the Matrix3 class")
{
	CHECK(Matrix3::ElementCount == 9);
	Matrix3 a = { 1, 0,  -4,
				  2, 6.5, 8,
				  9, 2,   6 };
	Matrix3 b = { 1, 5,  4,
				  3, 2,  4,
				  5, 2, -3 };

	SUBCASE("Testing Addition")
	{
		Matrix3 c = { 2, 5, 0,   5, 8.5, 12,    14, 4, 3 };
		CHECK(a + b == c);

		Matrix3 d = { 4, 7, 2,   7, 10.5, 14,   16, 6, 5 };
		CHECK(c + 2 == d);
	}
	SUBCASE("Testing Subtraction")
	{
		Matrix3 c = { 0, -5, -8,   -1, 4.5, 4,    4, 0, 9 };
		CHECK(a - b == c);

		Matrix3 d = { -2, -7, -10,  -3, 2.5, 2,   2, -2, 7 };
		CHECK(c - 2 == d);
	}
	SUBCASE("Testing Multiplication")
	{
		Matrix3 c = { -19, -3, 16,   61.5, 39, 10,    45, 61, 26 };
		CHECK(a * b == c);

		Matrix3 d = { 2, 0, -8,   4, 13, 16,    18, 4, 12 };
		CHECK(a * 2 == d);
	}
	SUBCASE("Testing Division")
	{
		Matrix3 c = { 0.5, 0, -2,   1, 3.25, 4,    4.5, 1, 3 };
		CHECK(a / 2 == c);
	}
	SUBCASE("Testing Transpose")
	{
		Matrix3 c = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		Matrix3 d = { 1, 4, 7, 2, 5, 8, 3, 6, 9 };
		CHECK(c.Transpose() == d);
	}
	SUBCASE("Testing Cofactor")
	{

	}
	SUBCASE("Testing Determinant")
	{

	}
	SUBCASE("Testing Inverse")
	{
		CHECK(CheckWithinBounds(a.Inverse() * a, Matrix3::Identity()));

		//NOTE(Ian): Assert currently fails, but not sure what the proper result should be
		//Matrix3 c = {};
		//CHECK(c * c.Inverse() == Matrix3::Identity());
	}
	SUBCASE("Testing Transform")
	{
		Matrix3 c = Matrix3::CreateScale( 2, 3 );
		Vector2 vec = { 4, 7 };
		Vector2 result = { 8, 21 };
		CHECK(c.Transform(vec) == result);
	}
}
/*
*/

#endif