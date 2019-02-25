#ifndef HANDMADE_QUATERNION_CPP
#define HANDMADE_QUATERNION_CPP

#include "Quaternion.h"

Quaternion Quaternion::Normalize() const
{
	return *this / this->Magnitude();
}

real32 Quaternion::Magnitude() const
{
	return sqrtf(MagnitudeSquared());
}

real32 Quaternion::MagnitudeSquared() const
{
	return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
}

Quaternion Quaternion::Inverse() const
{
	Quaternion result = { -(this->x), -(this->y), -(this->z), this->w };
	return result / this->Magnitude();
}

Quaternion Quaternion::CreateFromAxisAngle(Vector3 axis, real32 angle)
{
	Quaternion result = {};

	real32 halfSineAngle = sinf(angle) / 2;
	result.x = axis.x * halfSineAngle;
	result.y = axis.y * halfSineAngle;
	result.z = axis.z * halfSineAngle;
	result.w = cosf(angle) / 2;

	return result.Normalize();
}

//TODO: Copied from Ogre3D, replace this with something you actually understand later
Quaternion Quaternion::FromRotationMatrix(const Matrix4& kRot)
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

Matrix4 Quaternion::ToRotationMatrix() const
{
	real32 xx = x * x;
	real32 yy = y * y;
	real32 zz = z * z;

	real32 xy = x * y;
	real32 xz = x * z;
	real32 xw = x * w;
	
	real32 yz = y * z;
	real32 yw = y * w;

	real32 zw = z * w;

	Matrix4 rotation = 
	{	1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xz + yw), 0,
		    2 * (xy + zw), 1 - 2 * (xx + zz),     2 * (yz - xw), 0,
			2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), 0,
			0,                 0,                 0,             1 };

	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm

	return rotation;
}

/*static Quaternion Lerp(const Quaternion &start, const Quaternion &end, const real32 &fraction)
{

}

static Quaternion CosInterpolate(const Quaternion &start, const Quaternion &end, const real32 &fraction)
{

}*/

Vector3 Quaternion::Transform(const Vector3 &b) const
{
	Vector3 uv, uuv;
	Vector3 qvec = { x, y, z };
	uv = qvec.Cross(b) * 2.0f;
	uuv = qvec.Cross(uv);

	return b + uv * w + uuv;
}

Vector3 operator*(const Quaternion &a, const Vector3 &b)
{
	Vector3 uv, uuv;
	Vector3 qvec = { a.x, a.y, a.z };
	uv = qvec.Cross(b) * 2.0f;
	uuv = qvec.Cross(uv);

	return b + uv * a.w + uuv;
}

Quaternion operator*(const Quaternion &a, const Quaternion &b)
{
	Quaternion result;
	result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	result.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
	result.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
	result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	return result;

	/*
	Let Q1 and Q2 be two quaternions, which are defined, respectively, as (w1, x1, y1, z1) and (w2, x2, y2, z2).
	(Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
	(Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
	(Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
	(Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2

	https://www.cprogramming.com/tutorial/3d/quaternions.html
	http://www.ashwinnarayan.com/post/how-to-integrate-quaternions/
	*/
}

Quaternion operator*(const Quaternion &a, const real32 &b)
{
	Quaternion result = { a.x * b, a.y * b, a.z * b, a.w * b };
	return result;
}

Quaternion operator/(const Quaternion &a, const real32 &b)
{
	Quaternion result = { a.x / b, a.y / b, a.z / b, a.w / b };
	return result;
}

Quaternion& operator*=(Quaternion &a, const Quaternion &b)
{
	a = a * b;
	return a;
}

Quaternion& operator*=(Quaternion &a, const real32 &b)
{
	a = a * b;
	return a;
}

Quaternion& operator/=(Quaternion &a, const real32 &b)
{
	a = a / b;
	return a;
}

bool operator==(const Quaternion &a, const Quaternion &b)
{
	for (int32 i = 0; i < Quaternion::ElementCount; i++)
	{
		if (a.elements[i] != b.elements[i])
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const Quaternion &a, const Quaternion &b)
{
	return !(a == b);
}

#endif // !HANDMADE_QUATERNION_CPP