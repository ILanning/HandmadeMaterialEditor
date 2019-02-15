#ifndef HANDMADE_QUATERNION_H
#define HANDMADE_QUATERNION_H

#include <math.h>
#include "..\handmade_typedefs.h"
#include "Vector3.h"
#include "Matrix4.h"

struct Quaternion
{
	static const int ElementCount = 4;

	union
	{
		struct
		{
			real32 x, y, z, w;
		};
		real32 elements[ElementCount];
	};

	///Gives the quaterion a magnitude of 1 without changing its rotation.
	Quaternion Normalize() const;
	///Returns the length of the quaterion.
	real32 Magnitude() const;
	///Returns the squared length of the quaterion.
	real32 MagnitudeSquared() const;
	Quaternion Inverse() const;
	///Uses the quaternion to rotate the vector.
	Vector3 Transform(const Vector3 &b) const;

	static Quaternion Identity()
	{
		return { 0, 0, 0, 1 };
	}
	static Quaternion CreateFromAxisAngle(Vector3 axis, real32 angle);
	//TODO: Copied from Ogre3D, replace this with something you actually understand later
	static Quaternion FromRotationMatrix(const Matrix4& kRot);
	Matrix4 ToRotationMatrix() const;

	static Quaternion Lerp(const Quaternion &start, const Quaternion &end, const real32 &fraction);
	static Quaternion CosInterpolate(const Quaternion &start, const Quaternion &end, const real32 &fraction);

	friend Vector3 operator*(const Quaternion &a, const Vector3 &b);
	friend Quaternion operator*(const Quaternion &a, const Quaternion &b);
	friend Quaternion operator*(const Quaternion &a, const real32 &b);
	//TODO: Why does this give an unresolved external error when actually used elsewhere?
	friend Vector3 operator*(const Quaternion &a, const Vector3 &b);
	friend Quaternion operator/(const Quaternion &a, const real32 &b);
	friend Quaternion& operator*=(Quaternion &a, const Quaternion &b);
	friend Quaternion& operator*=(Quaternion &a, const real32 &b);
	friend Quaternion& operator/=(Quaternion &a, const real32 &b);

	friend Vector3 operator*(Quaternion &a, Vector3 &b);

	friend bool operator==(const Quaternion &a, const Quaternion &b);
	friend bool operator!=(const Quaternion &a, const Quaternion &b);
};

#endif //HANDMADE_QUATERNION_H