#ifndef HANDMADE_TEST_VECTOR3
#define HANDMADE_TEST_VECTOR3

#include "../../math/Vector3.h"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libraries/doctest.h"
#endif

TEST_CASE("Testing the Vector3 class")
{
	Vector3 a = { 2.0f, 3, 4 };
	REQUIRE(a.x == 2.0f);
	REQUIRE(a.y == 3.0f);
	REQUIRE(a.z == 4.0f);

	Vector3 b = { 5, 10, 6 };	

	SUBCASE("Testing equality")
	{
		Vector3 c = { 2, 3, 4 };
		CHECK(a == c);
		CHECK(a != b);
	}

	SUBCASE("Testing addition")
	{
		Vector3 c = a + b;
		Vector3 result = { 7, 13, 10 };
		CHECK(c == result);

		c = a + 3;
		result = { 5, 6, 7 };
		CHECK(c == result);

		c += 4;
		result = { 9, 10, 11 };
		CHECK(c == result);

		c += a;
		result = { 11, 13, 15 };
		CHECK(c == result);
	}

	SUBCASE("Testing subtraction")
	{
		Vector3 c = a - b;
		Vector3 result = { -3, -7, -2 };
		CHECK(c == result);

		c = a - 3;
		result = { -1, 0, 1 };
		CHECK(c == result);

		c -= 4;
		result = { -5, -4, -3 };
		CHECK(c == result);

		c -= a;
		result = { -7, -7, -7 };
		CHECK(c == result);
	}

	SUBCASE("Testing multiplication")
	{
		Vector3 c = a * 4;
		Vector3 result = { 8, 12, 16 };
		CHECK(c == result);

		c *= 2;
		result = { 16, 24, 32 };
		CHECK(c == result);
	}

	SUBCASE("Testing division")
	{
		Vector3 c = a / 2;
		Vector3 result = { 1, 1.5f, 2 };
		CHECK(c == result);

		c /= 2;
		result = { 0.5f, 0.75f, 1 };
		CHECK(c == result);
	}

	SUBCASE("Testing Normalize()")
	{
		Vector3 c = { 3, 0, 0 };
		c = c.Normalize();
		Vector3 result = { 1, 0, 0 };
		CHECK(c == result);

		c = { 10, 10, 10 };
		c = c.Normalize();
		result = { sqrtf(3.0f) / 3, sqrtf(3.0f) / 3, sqrtf(3.0f) / 3 };
		CHECK(c == result);

		c = { 0, 0, 0 };
		c = c.Normalize();
		result = { 0, 0, 0 };
		CHECK(c == result);
	}

	SUBCASE("Testing Dot()")
	{
		CHECK(a.Dot(b) == sqrtf(64));

		Vector3 c = { 0, 0 };
		CHECK(c.Dot(c) == 0);
	}

	SUBCASE("Testing Cross()")
	{
		Vector3 c = {-22, -8, -5};
		CHECK(a.Cross(b) == c);

		c = { 0, 0, 0 };
		CHECK(a.Cross(a) == c);
		CHECK(c.Cross(c) == c);
	}

	SUBCASE("Testing Magnitude()")
	{
		Vector3 c = { 3, 0, 4 };
		CHECK(c.Magnitude() == 5.0f);

		c = { -3, 4, 0 };
		CHECK(c.Magnitude() == 5.0f);

		c = { 0, 0, 0 };
		CHECK(c.Magnitude() == 0.0f);
	}

	SUBCASE("Testing MagnitudeSquared()")
	{
		Vector3 c = { 3, 0, 4 };
		CHECK(c.MagnitudeSquared() == 25.0f);

		c = { -3, 4, 0 };
		CHECK(c.MagnitudeSquared() == 25.0f);

		c = { 0, 0, 0 };
		CHECK(c.MagnitudeSquared() == 0.0f);
	}

	SUBCASE("Testing Lerp()")
	{
		a = { 0, 0, 0 };
		b = { 2, 10, 8 };
		Vector3 c = { 1, 5, 4 };
		CHECK(Vector3::Lerp(a, b, 0.5f) == c);

		a = { -1, -3, -5 };
		b = { -9, -12, -15 };
		c = { -5, -7.5f, -10 };
		CHECK(Vector3::Lerp(a, b, 0.5f) == c);

		a = { 0, 0, 0 };
		b = { 1, 0, 0 };
		c = { 4, 0, 0 };
		CHECK(Vector3::Lerp(a, b, 4) == c);

		a = { 0, 0, 0 };
		b = { 0, 0, 0 };
		c = { 0, 0, 0 };
		CHECK(Vector3::Lerp(a, b, 4) == c);
	}

	SUBCASE("Testing CosInterpolate()")
	{
		a = { 0, 0, 0 };
		b = { 2, 10, 8 };
		Vector3 c = { 1, 5, 4 };
		CHECK(Vector3::CosInterpolate(a, b, 0.5f) == c);
	}
}
#endif