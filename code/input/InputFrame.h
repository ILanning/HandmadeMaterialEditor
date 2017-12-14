#ifndef HANDMADE_INPUTFRAME_H
#define HANDMADE_INPUTFRAME_H

#include "../handmade_typedefs.h"
#include "PhysicalInputs.h"
#include "../math/Vector2.h"

namespace Input
{
	struct InputFrame
	{
		static const int32 KeyArraySize = PHYSICALINPUTS_LAST / 32 + 1;
		//Bitflag array of pressed keys
		bool32 PressedKeys[KeyArraySize];
		Vector2 MousePos;
		Vector2 BallisticsMousePos;

		bool GetKey(PhysicalInputs key)
		{
			return (PressedKeys[(int32)key / 32] & 1 << ((int32)key % 32)) != 0;
		}

		void SetKey(PhysicalInputs key, bool value)
		{
			int32 keyLocation = (int32)key / 32;
			if (value)
			{
				PressedKeys[keyLocation] = PressedKeys[keyLocation] | 1 << ((int32)key % 32);
			}
			else
			{
				PressedKeys[keyLocation] = PressedKeys[keyLocation] & 0 << ((int32)key % 32);
			}
		}
	};
}

#endif