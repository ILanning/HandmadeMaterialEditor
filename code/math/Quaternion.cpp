Quaternion Quaternion::Normalize()
{
	return *this / this->Magnitude();
}

real32 Quaternion::Magnitude()
{
	return sqrtf(MagnitudeSquared());
}

real32 Quaternion::MagnitudeSquared()
{
	return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
}

Quaternion Quaternion::Inverse()
{
	Quaternion result = { -(this->x), -(this->y), -(this->z), this->w };
	return result / this->Magnitude();
}

/*static Quaternion Lerp(const Quaternion &start, const Quaternion &end, const real32 &fraction)
{

}

static Quaternion CosInterpolate(const Quaternion &start, const Quaternion &end, const real32 &fraction)
{

}*/

friend Quaternion operator*(const Quaternion &a, const Quaternion &b)
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

friend Quaternion operator*(const Quaternion &a, const real32 &b)
{
	Quaternion result = { a.x * b, a.y * b, a.z * b, a.w * b };
	return result;
}

friend Quaternion operator/(const Quaternion &a, const real32 &b)
{
	Quaternion result = { a.x / b, a.y / b, a.z / b, a.w / b };
	return result;
}

friend Quaternion& operator*=(Quaternion &a, const Quaternion &b)
{
	a = a * b;
	return &a;
}

friend Quaternion& operator*=(Quaternion &a, const real32 &b)
{
	a = a * b;
	return &a;
}

friend Quaternion& operator/=(Quaternion &a, const real32 &b)
{
	a = a / b;
	return &a;
}

friend bool operator==(const Quaternion &a, const Quaternion &b)
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

friend bool operator!=(const Quaternion &a, const Quaternion &b)
{
	return !(a == b);
}