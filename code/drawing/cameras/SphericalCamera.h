#ifndef HANDMADE_SPHERICALCAMERA
#define HANDMADE_SPHERICALCAMERA

#include <math.h>
#include "..\..\general\Assert.h"
#include "..\..\math\Matrix4.h"
#include "..\..\math\Quaternion.h"
#include "..\..\math\Vector3.h"
#include "..\..\math\SphericalCoord.h"
#include "..\..\input\InputManager.h"
#include "..\..\input\PhysicalInputs.h"
#include "Camera.h"

namespace Drawing
{
	struct SphericalCamera
	{
		Matrix4 Projection;
		Vector3 lookAtPosition = { 0, 0, 0 };
		Math::SphericalCoord upperBound = { NAN, NAN, Pi32 / 4 - 0.01f, NAN };
		Math::SphericalCoord lowerBound = { NAN, NAN, -(Pi32 / 4 - 0.01f), NAN };

		Math::SphericalCoord targetCamPos = { 0, 0, 0, 0 };
		Math::SphericalCoord cameraPos = { 0, 0, 0 };
		real32 &radius = cameraPos.radius;
		real32 &direction = cameraPos.direction;
		real32 &elevation = cameraPos.elevation;
		real32 &roll = cameraPos.roll;

		real32 rotateSpeed = 0.10f;
		real32 moveSpeed = 0.07f;
		real32 zoomSpeed = 0.08f;
		real32 catchUpRate = 0.55f;

		virtual Vector3 GetLookAtPosition() const
		{
			return lookAtPosition;
		}
		virtual void SetLookAtPosition(const Vector3 &newPosition)
		{
			lookAtPosition = newPosition;
		}

		virtual Vector3 GetCameraPosition() const
		{
			return lookAtPosition + Math::SphericalCoord::ToCartesian(cameraPos);
		}
		virtual void SetCameraPosition(const Vector3 &newPosition)
		{
			Vector3 relative = newPosition - lookAtPosition;
			cameraPos = Math::SphericalCoord::FromCartesian(relative);
			targetCamPos = cameraPos;
		}

		real32 GetZoom() const
		{
			return radius;
		}
		void SetZoom(real32 newZoom)
		{
			radius = newZoom;
			targetCamPos.radius = newZoom;
		}

		Vector3 GetLookVector() const
		{
			return Math::SphericalCoord::ToCartesian(-1, direction, elevation);
		}

		Matrix4 GetView() const
		{
			Matrix4 rotation = Matrix4::CreateRotationZ(roll) * Matrix4::CreateRotationX(-elevation) * Matrix4::CreateRotationY(-direction);
			return Matrix4::CreateTranslation({ 0, 0, -radius }) * rotation * Matrix4::CreateTranslation(lookAtPosition * -1);
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

		void Rotate(const Quaternion &spin)
		{
			Assert(false); //Not implemented error
		}

		void HandleInput(const Input::InputManager &input)
		{
			using Keys = Input::PhysicalInputs;

			if (input.IsDown(Keys::S)) //About X axis
			{
				targetCamPos.elevation -= rotateSpeed;
			}
			if (input.IsDown(Keys::W))
			{
				targetCamPos.elevation += rotateSpeed;
			}
			if (input.IsDown(Keys::A)) //About Y axis
			{
				targetCamPos.direction -= rotateSpeed;
			}
			if (input.IsDown(Keys::D))
			{
				targetCamPos.direction += rotateSpeed;
			}
			if (input.IsDown(Keys::E)) //About Z Axis
			{
				targetCamPos.roll += rotateSpeed;
			}
			if (input.IsDown(Keys::Q))
			{
				targetCamPos.roll -= rotateSpeed;
			}

			int32 scroll = input.GetScrollValue();
			if (scroll > 0 || input.IsDown(Keys::U)) //Zoom level
			{
				targetCamPos.radius -= zoomSpeed * radius;
			}
			if (scroll < 0 || input.IsDown(Keys::J))
			{
				targetCamPos.radius += zoomSpeed * radius;
			}
			if (input.IsDown(Keys::RMB))
			{
				Vector2 mouseMove = input.GetMouseMovement() * rotateSpeed / 6;
				targetCamPos.elevation += mouseMove.y;
				targetCamPos.direction += mouseMove.x;
			}

			Math::SphericalCoord normalizedTarget = targetCamPos.Simplify();
			cameraPos -= targetCamPos - normalizedTarget;
			targetCamPos = normalizedTarget;

			if (input.IsDown(Keys::R))
			{
				targetCamPos = { 3, 0, Pi32 / 8, 0 };
			}

			if (cameraPos != targetCamPos)
			{
				if (cameraPos.CloseTo(targetCamPos, 0.001f))
				{
					cameraPos = targetCamPos;
				}
				else
				{
					cameraPos = Math::SphericalCoord::Slerp(cameraPos, targetCamPos, catchUpRate);
				}
			}

			Vector3 moveDirection = {};
			if (input.IsDown(Keys::Right))
			{
				moveDirection.x++;
			}
			if (input.IsDown(Keys::Left))
			{
				moveDirection.x--;
			}
			if (input.IsDown(Keys::Up))
			{
				moveDirection.z--;
			}
			if (input.IsDown(Keys::Down))
			{
				moveDirection.z++;
			}
			if (input.IsDown(Keys::PageUp))
			{
				moveDirection.y++;
			}
			if (input.IsDown(Keys::PageDown))
			{
				moveDirection.y--;
			}

			if (moveDirection.MagnitudeSquared() != 0)
			{
				moveDirection = moveDirection.Normalize() * moveSpeed * radius;
				moveDirection = Matrix4::CreateRotationY(direction).Transform(moveDirection);
				lookAtPosition += moveDirection;
			}
		}

		void Update() 
		{
		}
	};
}

#endif