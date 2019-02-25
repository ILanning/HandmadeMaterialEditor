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

		///Produces the cartesian equivalent of the given spherical coordinate.
		static Vector3 ToCartesian(real32 radius, Radian direction, Radian elevation);
		///Produces the cartesian equivalent of the given spherical coordinate.
		static Vector3 ToCartesian(const SphericalCoord &coords);
		///Creates the shperical version of the given cartesian coordinate.
		static SphericalCoord FromCartesian(const Vector3 &coords);
		///Creates a rotation quaternion from this spherical coordinate.
		static Quaternion ToNormalizedQuaternion(real32 radius, Radian direction, Radian elevation);
		///Creates a rotation quaternion from this spherical coordinate.
		static Quaternion ToNormalizedQuaternion(const SphericalCoord &coords);

		///Returns true if the other coordinate is close to this one, within the given margin.
		bool CloseTo(const SphericalCoord &other, real32 margin) const;
		///Reduces the size of the angle values as much as possible without changing their direction.
		SphericalCoord Simplify() const;
		///Returns the coordinate to within certain bounds.
		SphericalCoord Clamp(const SphericalCoord &upperBounds, const SphericalCoord &lowerBounds) const;

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
		///-1 on Y axis.
		static SphericalCoord Down() { return{ 1, 0, -Pi32, 0 }; }
		///-1 on X axis.
		static SphericalCoord Left() { return{ 1, Pi32 + Pi32 / 2, 0, 0 }; }
		///+1 on X axis.
		static SphericalCoord Right() { return{ 1, Pi32 / 2, 0, 0 }; }
		///+1 on Z axis.
		static SphericalCoord Forward() { return{ 1, 0, 0, 0 }; }
		///-1 on Z axis.
		static SphericalCoord Back() { return{ 1, Pi32, 0, 0 }; }

		static SphericalCoord Zero() { return{ 0, 0, 0, 0 }; }
	};
}

#endif // !HANDMADE_SPHERICALCOORD_H
