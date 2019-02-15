#ifndef HANDMADE_INPUTFRAME_H
#define HANDMADE_INPUTFRAME_H

#include "../handmade_typedefs.h"
#include "PhysicalInputs.h"
#include "../math/Vector2.h"

namespace Input
{
	///Represents a single frame of keyboard input.
	struct InputFrame
	{
		///The size of the key array.
		static const int32 KeyArraySize = PHYSICALINPUTS_LAST / 32 + 1;
		///Bitflag array of pressed keys.
		bool32 PressedKeys[KeyArraySize] = {};
		///The mouse psotion in this frame.
		Vector2 MousePos = { 0, 0 };
		///The raw number of pixels that the mouse moved, without OS weighting.
		Vector2 TrueMouseMovement = { 0, 0 };
		///The amount that the scroll wheel scrolled.
		int32 scrollWheelChange = 0;

		///Returns the value of a given key, true for pressed and false for unpressed.
		bool GetKey(PhysicalInputs key) const
		{
			return (PressedKeys[(int32)key / 32] & 1 << ((int32)key % 32)) != 0;
		}

		///Sets a key as being pressed or released.
		void SetKey(PhysicalInputs key, bool value)
		{
			int32 keyLocation = (int32)key / 32;
			if (value)
			{
				PressedKeys[keyLocation] = PressedKeys[keyLocation] | 1 << ((int32)key % 32);
			}
			else
			{
				PressedKeys[keyLocation] = PressedKeys[keyLocation] & ~(1 << ((int32)key % 32));
			}
		}
	};
}

#endif //HANDMADE_INPUTFRAME_H