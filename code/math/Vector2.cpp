union Vector2
{
	struct
	{
		real32 x, y;
	};
	struct
	{
		real32 u, v;
	};

	Vector2 Normalize();
	Vector2 operator+(const Vector2 &b);
	Vector2 operator-(const Vector2 &b);
	Vector2 operator+(const int32 &b);
	Vector2 operator-(const int32 &b);
};