#ifndef HANDMADE_INPUTMANAGER_H
#define HANDMADE_INPUTMANAGER_H

#include "../handmade_typedefs.h"
#include "../handmade.h"
#include "PhysicalInputs.h"
#include "InputFrame.h"
#include "../math/Vector2.cpp"

namespace Input
{
	class InputManager
	{
		//TODO(Ian): Keep a set of frames per registered controller
		//TODO(Ian): Provide a way to check whether or not an Action (Gesture?) was completed, as opposed to a PhysicalInput
		//           Actions could be associated with zero or more keys, or even a more complex input sequence that occurs over several frames
		InputFrame oldFrame = {};
		InputFrame newFrame = {};

	public:

		/**Updates the InputManager's state.
		*/
		void HandleInput(const GameInput *nextInputs)
		{
			oldFrame = newFrame;
			newFrame = nextInputs->newFrame;
		}

		/**Returns true if the given button is held.
		*/
		bool IsDown(const PhysicalInputs input) const
		{
			return newFrame.GetKey(input);
		}

		/**Returns true if all of the given buttons are held.
		*/
		template<typename... Args>
		bool IsDown(const PhysicalInputs a, const Args... args) const
		{
			return IsDown(a) && IsDown(args...);
		}

		/**Returns true if the given button was held last frame.
		*/
		bool WasDown(const PhysicalInputs input) const
		{
			return oldFrame.GetKey(input);
		}

		/**Returns true if all of the given buttons were held last frame.
		*/
		template<typename... Args>
		bool WasDown(const PhysicalInputs a, const Args... args) const
		{
			return WasDown(a) && WasDown(args...);
		}


		/**Returns true if the given button is not held.
		*/
		bool IsUp(const PhysicalInputs input) const
		{
			return !IsDown(input);
		}

		/**Returns true if all of the given buttons are not held.
		*/
		template<typename... Args>
		bool IsUp(const PhysicalInputs a, const Args... args) const
		{
			return IsUp(a) && IsUp(args...);
		}

		/**Returns true if the given button was not held last frame.
		*/
		bool WasUp(const PhysicalInputs input) const
		{
			return !WasDown(input);
		}

		/**Returns true if all of the given buttons were not held last frame.
		*/
		template<typename... Args>
		bool WasUp(const PhysicalInputs a, const Args... args) const
		{
			return WasUp(a) && WasUp(args...);
		}


		/**Returns true if the given button is held this frame, but was not last frame.
		*/
		bool IsTriggered(const PhysicalInputs input) const
		{
			return newFrame.GetKey(input) && !oldFrame.GetKey(input);
		}

		/**Returns true if at least one of the given inputs was triggered this frame.
		*/
		bool AtLeastOneTriggered(PhysicalInputs input) const
		{
			return IsTriggered(input);
		}

		/**Returns true if at least one of the given inputs was triggered this frame.
		*/
		template<typename... Args>
		bool AtLeastOneTriggered(const PhysicalInputs a, const Args... args) const
		{
			return IsTriggered(a) || AtLeastOneTriggered(args...);
		}

		/**Returns true if all the given buttons are held, and at least one was triggered this frame.
		*/
		template<typename... Args>
		bool ComboTriggered(const Args... args) const
		{
			return IsDown(args...) && AtLeastOneTriggered(args...);
		}


		/**Returns true if the given button is up this frame, but was held last frame.
		*/
		bool IsReleased(const PhysicalInputs input) const
		{
			return !newFrame.GetKey(input) && oldFrame.GetKey(input);
		}

		/**Returns true if at least one of the given inputs was released this frame.
		*/
		bool AtLeastOneReleased(PhysicalInputs input) const
		{
			return IsReleased(input);           
		}

		/**Returns true if at least one of the given inputs was released this frame.
		*/
		template<typename... Args>
		bool AtLeastOneReleased(const PhysicalInputs a, const Args... args) const
		{
			return IsReleased(a) || AtLeastOneReleased(args...);
		}

		/**Returns true if all the given buttons were held, and at least one was released this frame.
		*/
		template<typename... Args>
		bool ComboReleased(const PhysicalInputs a, const Args... args) const
		{
			if (WasDown(a) && WasDown(args...))
			{
				return IsTriggered(a) || AtLeastOneTriggered(args...);
			}
			return false;
		}

		Vector2 GetMousePosition() const
		{
			return newFrame.MousePos;
		}

		Vector2 GetMouseMovement() const
		{
			return newFrame.MousePos - oldFrame.MousePos;
		}

		int32 GetScrollValue() const
		{
			return newFrame.scrollWheelChange;
		}
	};
}

#endif //HANDMADE_INPUTMANAGER_H