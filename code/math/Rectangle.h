#ifndef HANDMADE_RECTANGLE_H
#define HANDMADE_RECTANGLE_H

#include "..\handmade_typedefs.h"
#include "Vector2.h"

namespace Math
{
	///A simple struct that contains the data for an axis aligned rectangle.
	struct Rectangle
	{
		///The total number of elements in the rectangle.
		const static int32 ElementCount = 4;
		union
		{
			struct
			{
				real32 x;
				real32 y;
				real32 width;
				real32 height;
			};
			///The Rectangle's data in an iterable format.
			real32 elements[4];
		};

		///Tests for intersection/collision between this Rectangle and another, edge inclusive.
		bool Intersect(const Rectangle &other) const
		{
			return ((other.x >= x && other.x <= x + width) ||
				(other.x + other.width >= x && other.x + other.width <= x + width)) &&
				((other.y >= y && other.y <= y + height) ||
				(other.y + other.height >= y && other.y + other.height <= y + height));
		}

		///Tests to see if the given Vector2 lies within this Rectangle, edge inclusive.
		bool Intersect(const Vector2 &vec) const
		{
			return vec.x >= x && vec.x <= x + width && vec.y >= y && vec.y <= y + height;
		}
	};
}

#endif // !HANDMADE_RECTANGLE_H
