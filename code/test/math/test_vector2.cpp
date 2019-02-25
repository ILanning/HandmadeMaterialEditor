#ifndef HANDMADE_TEST_VECTOR2
#define HANDMADE_TEST_VECTOR2

#include "../../math/Vector2.h"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libraries/doctest.h"
#endif

TEST_CASE("Testing the Vector2 class")
{
	Vector2 a = { 2.0f, 3 };
	REQUIRE(a.x == 2.0f);
	REQUIRE(a.y == 3.0f);

	Vector2 b = { 5, 10 };

	SUBCASE("Testing equality")
	{
		Vector2 c = { 2, 3 };
		CHECK(a == c);
		CHECK(a != b);
	}

	SUBCASE("Testing addition")
	{
		Vector2 c = a + b;
		Vector2 result = { 7, 13 };
		CHECK(c == result);

		c = a + 3;
		result = { 5, 6 };
		CHECK(c == result);

		c += 4;
		result = { 9, 10 };
		CHECK(c == result);

		c += a;
		result = { 11, 13 };
		CHECK(c == result);
	}

	SUBCASE("Testing subtraction")
	{
		Vector2 c = a - b;
		Vector2 result = { -3, -7 };
		CHECK(c == result);

		c = a - 3;
		result = { -1, 0 };
		CHECK(c == result);

		c -= 4;
		result = { -5, -4 };
		CHECK(c == result);

		c -= a;
		result = { -7, -7 };
		CHECK(c == result);
	}

	SUBCASE("Testing multiplication")
	{
		Vector2 c = a * 4;
		Vector2 result = { 8, 12 };
		CHECK(c == result);

		c *= 2;
		result = { 16, 24 };
		CHECK(c == result);
	}

	SUBCASE("Testing division")
	{
		Vector2 c = a / 2;
		Vector2 result = { 1, 1.5f };
		CHECK(c == result);

		c /= 2;
		result = { 0.5f, 0.75f };
		CHECK(c == result);
	}

	SUBCASE("Testing Normalize()")
	{
		Vector2 c = { 3, 0 };
		c = c.Normalize();
		Vector2 result = { 1, 0 };
		CHECK(c == result);

		c = { 10, 10 };
		c = c.Normalize();
		result = { sqrtf(2.0f)/2, sqrtf(2.0f)/2 };
		CHECK(c == result);

		c = { 0, 0 };
		c = c.Normalize();
		result = { 0, 0 };
		CHECK(c == result);
	}

	SUBCASE("Testing Dot()")
	{
		CHECK(a.Dot(b) == sqrtf(40));

		Vector2 c = { 0, 0 };
		CHECK(c.Dot(c) == 0);
	}

	SUBCASE("Testing Magnitude()")
	{
		Vector2 c = { 3, 4 };
		CHECK(c.Magnitude() == 5.0f);

		c = { -3, 4 };
		CHECK(c.Magnitude() == 5.0f);

		c = { 0, 0 };
		CHECK(c.Magnitude() == 0.0f);
	}

	SUBCASE("Testing MagnitudeSquared()")
	{
		Vector2 c = { 3, 4 };
		CHECK(c.MagnitudeSquared() == 25.0f);

		c = { -3, 4 };
		CHECK(c.MagnitudeSquared() == 25.0f);

		c = { 0, 0 };
		CHECK(c.MagnitudeSquared() == 0.0f);
	}

	SUBCASE("Testing Lerp()")
	{
		a = { 0, 0 };
		b = { 2, 10 };
		Vector2 c = { 1, 5 };
		CHECK(Vector2::Lerp(a, b, 0.5f) == c);

		a = { -1, -3 };
		b = { -9, -12 };
		c = { -5, -7.5f };
		CHECK(Vector2::Lerp(a, b, 0.5f) == c);

		a = { 0, 0 };
		b = { 1, 0 };
		c = { 4, 0 };
		CHECK(Vector2::Lerp(a, b, 4) == c);

		a = { 0, 0 };
		b = { 0, 0 };
		c = { 0, 0 };
		CHECK(Vector2::Lerp(a, b, 4) == c);
	}

	SUBCASE("Testing CosInterpolate()")
	{
		a = { 0, 0 };
		b = { 2, 10 };
		Vector2 c = { 1, 5 };
		CHECK(Vector2::CosInterpolate(a, b, 0.5f) == c);
	}
}

#endif