#ifndef HANDMADE_RECTANGLE_H
#define HANDMADE_RECTANGLE_H

struct Rectangle
{
	union
	{
		real32[4] elements;
		struct
		{
			real32 x;
			real32 y;
			real32 width;
			real32 height;
		};
	};

	bool Intersect(const Rectangle &other) const
	{
		return ((other.x >= x && other.x <= x + width) || 
				(other.x + other.width >= x && other.x + other.width <= x + width)) &&
			   ((other.y >= y && other.y <= y + height) ||
				(other.y + other.height >= y && other.y + other.height <= y + height));
	}

	bool Intersect(const Vector2 &vec) const
	{
		return vec.x >= x && vec.x <= x + width && vec.y >= y && vec.y <= y + height;
	}
};

#endif // !HANDMADE_RECTANGLE_H
