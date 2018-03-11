#ifndef HANDMADE_SPHERICALCOORD_CPP
#define HANDMADE_SPHERICALCOORD_CPP

#include "SphericalCoord.h"
#include <math.h>
#include "MathTypedefs.h"
#include "..\handmade_typedefs.h"
#include "Vector3.cpp"
#include "Quaternion.cpp"
#include "Matrix4.cpp"
#include "MathHelpers.cpp"

namespace Math
{

	Vector3 SphericalCoord::ToCartesian(real32 radius, Radian direction, Radian elevation)
	{
		real32 cosElevation = cosf(elevation);
		Vector3 coord = { cosElevation * cosf(direction), sinf(elevation), cosElevation * sinf(direction) };
		return coord * radius;
	}

	Vector3 SphericalCoord::ToCartesian(const SphericalCoord &coords)
	{
		return ToCartesian(coords.radius, coords.direction, coords.elevation);
	}

	SphericalCoord SphericalCoord::FromCartesian(const Vector3 &coords)
	{
		SphericalCoord result = { coords.Magnitude(), 0, 0, 0 };
		result.direction = atan2f(coords.x, coords.z);
		result.elevation = asinf(coords.y / result.radius);
		return result;
	}

	Quaternion SphericalCoord::ToNormalizedQuaternion(real32 radius, Radian direction, Radian elevation)
	{
		//TODO(Ian): Does not account for roll
		//Probably choose an up vector based on roll, then build axis by crossing the cartesian vector with that
		Vector3 axis = { cosf(direction), 0, sinf(direction) };
		return Quaternion::CreateFromAxisAngle(axis, elevation);
	}

	Quaternion SphericalCoord::ToNormalizedQuaternion(const SphericalCoord &coords)
	{
		return ToNormalizedQuaternion(coords.radius, coords.direction, coords.elevation);
	}

	bool SphericalCoord::CloseTo(const SphericalCoord &other, real32 margin) const
	{
		if (fabsf(radius - other.radius) > margin ||
			AngleDist(direction, other.direction) > margin ||
			AngleDist(elevation, other.elevation) > margin ||
			AngleDist(roll, other.roll) > margin)
		{
			return false;
		}

		return true;
	}

	/** Returns the coordinate to the range ([+infinity, 0], (2Pi, 0], (2Pi, 0]) without modifying the represented rotation.
	*/
	SphericalCoord SphericalCoord::Simplify() const
	{
		SphericalCoord result = *this;
		if (result.radius < 0)
		{
			result.radius = -result.radius;
			result.direction += Pi32 / 2;
			result.elevation = -result.elevation;
		}

		if (result.direction >= Pi32 || result.direction < -Pi32)
		{
			Radian fullSpins = floorf(result.direction / (Pi32 * 2));
			result.direction -= fullSpins * Pi32 * 2;
		}

		if (result.elevation >= Pi32 || result.elevation < -Pi32)
		{
			Radian fullSpins = floorf(result.elevation / (Pi32 * 2));
			result.elevation -= fullSpins * Pi32 * 2;
		}

		if (result.roll >= Pi32 || result.roll < -Pi32)
		{
			Radian fullSpins = floorf(result.roll / (Pi32 * 2));
			result.roll -= fullSpins * Pi32 * 2;
		}

		return result;
	}

	/** Clamps the given coordinate to the given range.  Use NaN for values with no intended limit.  If upperBounds.y is less than lowerBounds.y, the range will cross 0/2Pi.
	*/
	SphericalCoord SphericalCoord::Clamp(const SphericalCoord &upperBounds, const SphericalCoord &lowerBounds) const
	{
		SphericalCoord result = *this;

		if (result.radius > upperBounds.radius)
		{
			result.radius = upperBounds.radius;
		}
		else if (result.radius < lowerBounds.radius)
		{
			result.radius = lowerBounds.radius;
		}

		result.direction = ClampAngle(result.direction, upperBounds.direction, lowerBounds.direction);

		if (result.elevation > upperBounds.elevation)
		{
			result.elevation = upperBounds.elevation;
		}
		else if (result.elevation < lowerBounds.elevation)
		{
			result.elevation = lowerBounds.elevation;
		}

		result.roll = ClampAngle(result.roll, upperBounds.roll, lowerBounds.roll);

		return result;
	}

	SphericalCoord SphericalCoord::Slerp(const SphericalCoord &start, const SphericalCoord &end, real32 fraction)
	{
		SphericalCoord result = {};
		result.radius = start.radius + (end.radius - start.radius) * fraction;
		result.direction = Math::Slerp(start.direction, end.direction, fraction);
		result.elevation = Math::Slerp(start.elevation, end.elevation, fraction);
		result.roll = Math::Slerp(start.roll, end.roll, fraction);
		return result;
	}

	SphericalCoord SphericalCoord::CosInterpolate(const SphericalCoord &start, const SphericalCoord &end, real32 fraction)
	{
		real32 cosFraction = (1 - cosf(fraction * Pi32)) / 2;
		return Slerp(start, end, cosFraction);
	}

	SphericalCoord operator-(SphericalCoord a)
	{
		a.direction += Pi32 / 2;
		a.elevation = -a.elevation;
		return a;
	}

	SphericalCoord& operator+=(SphericalCoord &a, const SphericalCoord &b)
	{
		a.radius += b.radius;
		a.direction += b.direction;
		a.elevation += b.elevation;
		a.roll += b.roll;
		return a;
	}

	SphericalCoord& operator-=(SphericalCoord &a, const SphericalCoord &b)
	{
		a.radius -= b.radius;
		a.direction -= b.direction;
		a.elevation -= b.elevation;
		a.roll -= b.roll;
		return a;
	}

	SphericalCoord operator+(SphericalCoord a, const SphericalCoord &b)
	{
		a += b;
		return a;
	}

	SphericalCoord operator-(SphericalCoord a, const SphericalCoord &b)
	{
		a -= b;
		return a;
	}

	bool operator==(const SphericalCoord &a, const SphericalCoord &b)
	{
		return a.radius == b.radius && a.direction == b.direction && a.elevation == b.elevation && a.roll == b.roll;
	}
	bool operator!=(const SphericalCoord &a, const SphericalCoord &b)
	{
		return !(a == b);
	}
}

#endif // !HANDMADE_SPHERICALCOORD_CPP
