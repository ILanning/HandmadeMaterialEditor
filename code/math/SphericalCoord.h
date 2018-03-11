#ifndef HANDMADE_SPHERICALCOORD_H
#define HANDMADE_SPHERICALCOORD_H

#include "MathTypedefs.h"
#include "..\handmade_typedefs.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"

namespace Math
{
	struct SphericalCoord
	{
		static const int ElementCount = 4;

		union
		{
			struct
			{
				real32 radius, direction, elevation, roll;
			};
			real32 elements[ElementCount];
		};

		static Vector3 ToCartesian(real32 radius, Radian direction, Radian elevation);
		static Vector3 ToCartesian(const SphericalCoord &coords);
		static SphericalCoord FromCartesian(const Vector3 &coords);

		static Quaternion ToNormalizedQuaternion(real32 radius, Radian direction, Radian elevation);
		static Quaternion ToNormalizedQuaternion(const SphericalCoord &coords);

		bool CloseTo(const SphericalCoord &other, real32 margin) const;
		SphericalCoord SphericalCoord::Simplify() const;
		SphericalCoord SphericalCoord::Clamp(const SphericalCoord &upperBounds, const SphericalCoord &lowerBounds) const;

		static SphericalCoord Slerp(const SphericalCoord &start, const SphericalCoord &end, real32 fraction);
		static SphericalCoord CosInterpolate(const SphericalCoord &start, const SphericalCoord &end, real32 fraction);


		friend SphericalCoord operator-(SphericalCoord a);
		friend SphericalCoord& operator+=(SphericalCoord &a, const SphericalCoord &b);
		friend SphericalCoord& operator-=(SphericalCoord &a, const SphericalCoord &b);
		friend SphericalCoord operator+(SphericalCoord a, const SphericalCoord &b);
		friend SphericalCoord operator-(SphericalCoord a, const SphericalCoord &b);
		friend bool operator==(const SphericalCoord &a, const SphericalCoord &b);
		friend bool operator!=(const SphericalCoord &a, const SphericalCoord &b);

		static SphericalCoord Up() { return{ 1, 0, Pi32, 0 }; }
		static SphericalCoord Down() { return{ 1, 0, -Pi32, 0 }; }
		static SphericalCoord Left() { return{ 1, Pi32 + Pi32 / 2, 0, 0 }; }
		static SphericalCoord Right() { return{ 1, Pi32 / 2, 0, 0 }; }
		static SphericalCoord Forward() { return{ 1, 0, 0, 0 }; }
		static SphericalCoord Back() { return{ 1, Pi32, 0, 0 }; }

		static SphericalCoord Zero() { return{ 0, 0, 0, 0 }; }
	};
}

#endif // !HANDMADE_SPHERICALCOORD_H
