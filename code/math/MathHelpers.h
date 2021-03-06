#ifndef HANDMADE_MATHHELPERS_H
#define HANDMADE_MATHHELPERS_H

#include "MathTypedefs.h"
#include "../handmade_typedefs.h"
#include "../general/Assert.h"
#include <math.h>

namespace Math
{
	/** Linearly interpolate between two angles
	*/
	inline Radian Slerp(Radian start, Radian end, real32 fraction)
	{
		//If the difference between the two is greater than Pi, the shortest path crosses the 2Pi/0 boundary
		Radian diff = end - start;
		if (fabsf(diff) > Pi32)
		{
			//We need to mirror End across the axis defined by the angle of Start, then use the negative difference from that
			Radian oppStart = start;
			oppStart += (end < start ? -Pi32 : Pi32);

			end = oppStart + (oppStart - end);

			return start - ((end - start) * fraction);

		}
		return start + diff * fraction;
	}

	///Measures the distance between two angles
	inline Radian AngleDist(Radian a, Radian b)
	{
		Radian diff = fabsf(b - a);
		if (diff > Pi32)
		{
			Radian oppStart = a;
			oppStart += (b < a ? -Pi32 : Pi32);

			b = oppStart + (oppStart - b);

			return fabsf(b - a);
		}

		return diff;
	}

	inline Radian ClampAngle(Radian orig, Radian max, Radian min)
	{
		if (max >= min || max == NAN || min == NAN)
		{
			if (orig > max)
			{
				orig = max;
			}
			else if (orig < min)
			{
				orig = min;
			}
		}
		else //Clamp to range [max, 0][2Pi, min]
		{
			//TODO: Will fail to produce correct results if the bounds are out of (2Pi, 0], fix that
			if (orig >= Pi32 * 2 || orig < 0)
			{
				Radian fullSpins = floorf(orig / (Pi32 * 2));
				orig -= fullSpins * Pi32 * 2;
			}

			Radian halfway = (min - max) / 2 + max;

			if (orig > max && orig <= halfway)
			{
				orig = max;
			}
			else if (orig < min && orig > halfway)
			{
				orig = min;
			}
		}

		return orig;
	}

	inline uint32 SafeTruncateUInt64(uint64 Value)
	{
		Assert(Value <= MaxUInt32);
		uint32 Result = (uint32)Value;
		return(Result);
	}
}


#endif //HANDMADE_MATHHELPERS_H