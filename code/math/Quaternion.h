#ifndef HANDMADE_QUATERNION
#define HANDMADE_QUATERNION

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

	static Quaternion CreateFromAxisAngle(Vector3 axis, real32 angle)
	{
		Quaternion result = {};

		real32 halfSineAngle = sinf(angle) / 2;
		result.x = axis.x * halfSineAngle;
		result.y = axis.y * halfSineAngle;
		result.z = axis.z * halfSineAngle;
		result.w = cosf(angle) / 2;

		return result.Normalize();
	}

	//TODO(Ian): Copied from Ogre3D, replace this with something you actually understand later
	static Quaternion FromRotationMatrix(const Matrix4& kRot)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		real32 fTrace = kRot.m[0][0] + kRot.m[1][1] + kRot.m[2][2];
		real32 fRoot;

		Quaternion result = {};

		if (fTrace > 0.0)
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = sqrtf(fTrace + 1.0f);  // 2w
			result.w = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;  // 1/(4w)
			result.x = (kRot.m[2][1] - kRot.m[1][2])*fRoot;
			result.y = (kRot.m[0][2] - kRot.m[2][0])*fRoot;
			result.z = (kRot.m[1][0] - kRot.m[0][1])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if (kRot.m[1][1] > kRot.m[0][0])
				i = 1;
			if (kRot.m[2][2] > kRot.m[i][i])
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			fRoot = sqrtf(kRot.m[i][i] - kRot.m[j][j] - kRot.m[k][k] + 1.0f);
			//Real* apkQuat[3] = { &x, &y, &z };
			result.elements[i] = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			result.w = (kRot.m[k][j] - kRot.m[j][k])*fRoot;
			result.elements[j] = (kRot.m[j][i] + kRot.m[i][j])*fRoot;
			result.elements[k] = (kRot.m[k][i] + kRot.m[i][k])*fRoot;
		}

		return result;
	}

	Matrix4 ToRotationMatrix() const;

	Quaternion Normalize();
	real32 Magnitude();
	real32 MagnitudeSquared();
	Quaternion Inverse();
	Vector3 Transform(const Vector3 &b) const;

	static Quaternion Identity()
	{
		return { 0, 0, 0, 1 };
	}

	static Quaternion Lerp(const Quaternion &start, const Quaternion &end, const real32 &fraction);
	static Quaternion CosInterpolate(const Quaternion &start, const Quaternion &end, const real32 &fraction);

	friend Quaternion operator*(const Quaternion &a, const Quaternion &b);
	friend Quaternion operator*(const Quaternion &a, const real32 &b);
	//TODO(Why does this give an unresolved external error when actually used elsewhere?)
	friend Vector3 operator*(const Quaternion &a, const Vector3 &b);
	friend Quaternion operator/(const Quaternion &a, const real32 &b);
	friend Quaternion& operator*=(Quaternion &a, const Quaternion &b);
	friend Quaternion& operator*=(Quaternion &a, const real32 &b);
	friend Quaternion& operator/=(Quaternion &a, const real32 &b);

	friend Vector3 operator*(Quaternion &a, Vector3 &b);

	friend bool operator==(const Quaternion &a, const Quaternion &b);
	friend bool operator!=(const Quaternion &a, const Quaternion &b);
};

#include "Quaternion.cpp"

#endif