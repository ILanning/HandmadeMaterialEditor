Vector3 Vector3::Normalize()
{
	real32 mag = Magnitude();
	if (mag == 0)
	{
		return *this;
	}
	return *this / mag;
}

real32 Vector3::Dot(const Vector3 &other)
{
	return sqrtf(this->x * other.x + this->y * other.y + this->z * other.z);
}

Vector3 Vector3::Cross(const Vector3 &other)
{
	return{
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x,
	};
}

real32 Vector3::Magnitude()
{
	return sqrtf(MagnitudeSquared());
}

real32 Vector3::MagnitudeSquared()
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vector3 Vector3::Lerp(const Vector3 &start, const Vector3 &end, const real32 &fraction)
{
	return (end - start) * fraction + start;
}

Vector3 Vector3::CosInterpolate(const Vector3 &start, const Vector3 &end, const real32 &fraction)
{
	real32 cosFraction = (1 - cosf(fraction * Pi32)) / 2;
	return Lerp(start, end, cosFraction);
}

Vector3 operator+(const Vector3 &a, const Vector3 &b)
{
	return{ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 operator+(const Vector3 &a, const real32 &b)
{
	return{ a.x + b, a.y + b, a.z + b };
}

Vector3 operator-(const Vector3 &a, const Vector3 &b)
{
	return{ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 operator-(const Vector3 &a, const real32 &b)
{
	return{ a.x - b, a.y - b, a.z - b };
}

Vector3 operator*(const Vector3 &a, const real32 &b)
{
	return{ a.x * b, a.y * b, a.z * b };
}

Vector3 operator/(const Vector3 &a, const real32 &b)
{
	return { a.x / b, a.y / b, a.z / b };
}

Vector3& operator+=(Vector3 &a, const Vector3 &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3& operator+=(Vector3 &a, const real32 &b)
{
	a.x += b;
	a.y += b;
	a.z += b;
	return a;
}

Vector3& operator-=(Vector3 &a, const Vector3 &b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vector3& operator-=(Vector3 &a, const real32 &b)
{
	a.x -= b;
	a.y -= b;
	a.z -= b;
	return a;
}

Vector3& operator*=(Vector3 &a, const real32 &b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

Vector3& operator/=(Vector3 &a, const real32 &b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return a;
}

bool operator==(const Vector3 &a, const Vector3 &b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator!=(const Vector3 &a, const Vector3 &b)
{
	return !(a == b);
}