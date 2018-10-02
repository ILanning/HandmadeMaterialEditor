#ifndef HANDMADE_INPUT_CAMERA2D
#define HANDMADE_INPUT_CAMERA2D

#include "..\..\handmade_typedefs.h"
#include "..\..\input\InputManager.h"
#include "..\..\math\Matrix4.h"
#include "..\..\math\Vector3.h"
#include "..\..\math\Vector2.h"

namespace Drawing
{
	///A camera for drawing 2D assets, such as UI elements.
	class Camera2D
	{
	public:
		Vector3 Position = {};
		Vector3 targetPos = {};
		Vector2 Size = {};
		real32 Rotation = 0;
		Matrix4 Projection;
		real32 BoostFactor = 4.0f;
		real32 MoveSpeed = 20;
		real32 CatchUpRate = 0.50f;

		Camera2D(const Vector2 screenSize, const Vector2 topLeftPos) : Position({ topLeftPos.x, topLeftPos.y, 0.0f })
		{
			Projection = Matrix4::CreateOrthographic(screenSize.x, screenSize.y, -1000, 1000);
		}

		virtual Vector3 GetLookAtPosition() const
		{
			return Position;
		}
		virtual void SetLookAtPosition(const Vector3 &newPosition)
		{
			Position = newPosition;
		}

		virtual Vector3 GetCameraPosition() const
		{
			return GetLookAtPosition();
		}


		virtual void SetCameraPosition(const Vector3 &newPosition)
		{
			SetLookAtPosition(newPosition);
		}

		///Returns the point about which the camera swivels.
		Vector2 GetOrigin() const
		{
			return Size / 2;
		}

		///Returns the direction that the camera is looking in.
		Vector3 GetLookVector() const
		{
			return Vector3::Forward();
		}

		///Returns the camera's current view matrix.
		Matrix4 GetView() const
		{
			return Matrix4::CreateTranslation({ Position.x, Position.y, Position.z }) * Matrix4::CreateRotationZ(Rotation);
		}

		/// Returns the projection matrix for this camera.
		Matrix4 GetProjection() const
		{
			return Projection;
		}

		/// Sets the projection matrix for this camera.
		void SetProjection(const Matrix4 &newProjection)
		{
			Projection = newProjection;
		}

		/// Returns the view matrix multiplied by the projection matrix.
		Matrix4 GetViewProjection() const
		{
			return Projection * GetView();
		}

		///Allows the camera to react to user inputs.
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
		}

		///Allows the camera to preform any actions that occur regardless of the user.
		void Update()
		{
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
	};
}

#endif // !HANDMADE_INPUT_CAMERA2D
