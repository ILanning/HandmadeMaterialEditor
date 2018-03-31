#ifndef HANDMADE_INPUT_CAMERA2D
#define HANDMADE_INPUT_CAMERA2D

#include "..\..\handmade_typedefs.h"
#include "..\..\math\Matrix4.h"
#include "..\..\math\Vector3.h"
#include "..\..\math\Vector2.h"

namespace Input
{
	class Camera2D
	{
		Vector2 Position;
		Vector2 Size;
		Matrix4 Projection;
		real32 MoveSpeed;
		real32 CatchUpRate;

		virtual Vector3 GetLookAtPosition() const
		{
			return lookAtPosition;
		}
		virtual void SetLookAtPosition(const Vector3 &newPosition) //Ignore Z
		{
			lookAtPosition = newPosition;
		}

		virtual Vector3 GetCameraPosition() const
		{
			return GetLookAtPosition();
		}
		virtual void SetCameraPosition(const Vector3 &newPosition)
		{
			SetLookAtPosition(newPosition);
		}

		Vector2 GetOrigin() const
		{

		}

		void HandleInput()
		{

		}

		void Update(InputManager)
		{

		}
	};
}

#endif // !HANDMADE_INPUT_CAMERA2D
