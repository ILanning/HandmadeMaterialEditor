Vector2 Vector2::Normalize()
{
	real32 mag = Magnitude();
	if (mag == 0)
	{
		return *this;
	}
	return *this / mag;
}

real32 Vector2::Dot(const Vector2 &b)
{
	return sqrtf(this->x * b.x + this->y * b.y);
}

real32 Vector2::Magnitude()
{
	return sqrtf(MagnitudeSquared());
}

real32 Vector2::MagnitudeSquared()
{
	return this->x * this->x + this->y * this->y;
}

Vector2 Vector2::Lerp(const Vector2 &start, const Vector2 &end, const real32 &fraction)
{
	return (end - start) * fraction + start;
}

Vector2 Vector2::CosInterpolate(const Vector2 &start, const Vector2 &end, const real32 &fraction)
{
	real32 cosFraction = (1 - cosf(fraction * Pi32)) / 2;
	return Lerp(start, end, cosFraction);
}

Vector2 operator+(const Vector2 &a, const Vector2 &b)
{
	return{ a.x + b.x, a.y + b.y };
}

Vector2 operator-(const Vector2 &a, const Vector2 &b)
{
	return{ a.x - b.x, a.y - b.y };
}

Vector2 operator+(const Vector2 &a, const real32 &b)
{
	return{ a.x + b, a.y + b };
}

Vector2 operator-(const Vector2 &a, const real32 &b)
{
	return{ a.x - b, a.y - b };
}

Vector2 operator*(const Vector2 &a, const real32 &b)
{
	return{ a.x * b, a.y * b };
}

Vector2 operator/(const Vector2 &a, const real32 &b)
{
	return { a.x / b, a.y / b };
}

Vector2& operator+=(Vector2 &a, const Vector2 &b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vector2& operator+=(Vector2 &a, const real32 &b)
{
	a.x += b;
	a.y += b;
	return a;
}

Vector2& operator-=(Vector2 &a, const Vector2 &b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

Vector2& operator-=(Vector2 &a, const real32 &b)
{
	a.x -= b;
	a.y -= b;
	return a;
}

Vector2& operator*=(Vector2 &a, const real32 &b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

Vector2& operator/=(Vector2 &a, const real32 &b)
{
	a.x /= b;
	a.y /= b;
	return a;
}

bool operator==(const Vector2 &a, const Vector2 &b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(const Vector2 &a, const Vector2 &b)
{
	return !(a == b);
}