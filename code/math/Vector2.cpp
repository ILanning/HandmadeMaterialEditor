Vector2 Vector2::Normalize() const
{
	real32 mag = Magnitude();
	if (mag == 0)
	{
		return { 0, 0 };
	}
	return *this / mag;
}

real32 Vector2::Dot(const Vector2 &b) const
{
	return sqrtf(x * b.x + y * b.y);
}

real32 Vector2::Magnitude() const
{
	return sqrtf(MagnitudeSquared());
}

real32 Vector2::MagnitudeSquared() const
{
	return x * x + y * y;
}

bool Vector2::CloseTo(const Vector2 &other, real32 margin) const
{
	for (int32 i = 0; i < Vector2::ElementCount; i++)
	{
		real32 diff = elements[i] - other.elements[i];
		diff = diff < 0 ? -diff : diff;
		
		if (diff > margin)
		{
			return false;
		}
	}

	return true;
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