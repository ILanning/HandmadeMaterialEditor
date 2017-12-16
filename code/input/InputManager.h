#ifndef HANDMADE_INPUTMANAGER_H
#define HANDMADE_INPUTMANAGER_H

#include "../handmade_typedefs.h"
#include "PhysicalInputs.h"
#include "InputFrame.h"
#include "../math/Vector2.h"

namespace Input
{
	struct InputManager
	{
		//TODO(Ian): Keep a set of frames per registered controller
		//TODO(Ian): Provide a way to check whether or not an Action (Gesture?) was completed, as opposed to a PhysicalInput
		//           Actions could be associated with zero or more keys, or even a more complex input sequence that occurs over several frames
		InputFrame OldFrame = {};
		InputFrame NewFrame = {};

		void HandleInput(GameInput *nextInputs)
		{
			OldFrame = NewFrame;
			NewFrame = nextInputs->newFrame;
		}

		bool IsDown(PhysicalInputs input)
		{
			return NewFrame.GetKey(input);
		}

		bool IsUp(PhysicalInputs input)
		{
			return !IsDown(input);
		}

		bool IsTriggered(PhysicalInputs input)
		{
			return NewFrame.GetKey(input) && !OldFrame.GetKey(input);
		}

		bool IsReleased(PhysicalInputs input)
		{
			return !NewFrame.GetKey(input) && OldFrame.GetKey(input);
		}

		Vector2 GetMousePosition()
		{
			return NewFrame.MousePos;
		}

		Vector2 GetMouseMovement()
		{
			return NewFrame.MousePos - OldFrame.MousePos;
		}
	};
}

#endif