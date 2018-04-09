#ifndef HANDMADE_INPUT_CAMERA2D
#define HANDMADE_INPUT_CAMERA2D

#include "..\..\handmade_typedefs.h"
#include "..\..\input\InputManager.h"
#include "..\..\math\Matrix4.h"
#include "..\..\math\Vector3.h"
#include "..\..\math\Vector2.h"

namespace Drawing
{
	class Camera2D
	{
	public:
		Vector3 Position = {};
		Vector3 targetPos = {};
		Vector2 Size = {};
		Matrix4 Projection;
		real32 BoostFactor = 4.0f;
		real32 MoveSpeed = 20;
		real32 CatchUpRate = 0.40f;

		Camera2D(const Vector2 screenSize, const Vector2 topLeftPos) : Position({ topLeftPos.x, topLeftPos.y, 0.0f })
		{
			Projection = Matrix4::CreateOrthographic(screenSize.x, screenSize.y, -100, 100);
		}

		virtual Vector3 GetLookAtPosition() const
		{
			return Position;// { Position.x, Position.y, 0.0f };
		}
		virtual void SetLookAtPosition(const Vector3 &newPosition) //Ignore Z
		{
			Position = newPosition;// { newPosition.x, newPosition.y };
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
			return Size / 2;
		}

		Vector3 GetLookVector() const
		{
			return Vector3::Down();
		}

		Matrix4 GetView() const
		{
			return Matrix4::CreateTranslation({ Position.x, Position.y, Position.z });// *Matrix4::CreateRotationX(Pi32 / 4.0f);
		}

		/** Returns the projection matrix for this camera.
		*/
		Matrix4 GetProjection() const
		{
			return Projection;
		}
		/** Sets the projection matrix for this camera.
		*/
		void SetProjection(const Matrix4 &newProjection)
		{
			Projection = newProjection;
		}
		/** Returns the view matrix multiplied by the projection matrix.
		*/
		Matrix4 GetViewProjection() const
		{
			return Projection * GetView();
		}

		void HandleInput(const Input::InputManager &input)
		{
			using Keys = Input::PhysicalInputs;

			Vector3 moveDirection = {};
			if (input.IsDown(Keys::Right))
			{
				moveDirection.x--;
			}
			if (input.IsDown(Keys::Left))
			{
				moveDirection.x++;
			}
			if (input.IsDown(Keys::Up))
			{
				moveDirection.y--;
			}
			if (input.IsDown(Keys::Down))
			{
				moveDirection.y++;
			}
			if (input.IsDown(Keys::PageUp))
			{
				moveDirection.z++;
			}
			if (input.IsDown(Keys::PageDown))
			{
				moveDirection.z--;
			}

			if (moveDirection.MagnitudeSquared() != 0)
			{
				moveDirection = moveDirection.Normalize() * MoveSpeed;
				if (input.IsDown(Keys::Shift))
				{
					moveDirection *= BoostFactor;
				}
				targetPos += moveDirection;
			}

			Vector3 diff = targetPos - Position;
			if (diff.MagnitudeSquared() > 0.01f)
			{
				Position += diff * CatchUpRate;
			}
			else
			{
				Position = targetPos;
			}
		}

		void Update()
		{
			
		}
	};
}

#endif // !HANDMADE_INPUT_CAMERA2D
