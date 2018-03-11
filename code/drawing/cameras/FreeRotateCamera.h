#ifndef HANDMADE_FREEROTATECAMERA
#define HANDMADE_FREEROTATECAMERA

#include <math.h>
#include "..\..\general\Assert.h"
#include "..\..\math\Matrix4.h"
#include "..\..\math\Quaternion.h"
#include "..\..\math\Vector3.cpp"
#include "..\..\input\InputManager.h"
#include "..\..\input\PhysicalInputs.h"
#include "Camera.h"

namespace Drawing
{
	struct FreeRotateCamera : Camera
	{
		real32 zoomLevel = 1;
		Vector3 lookAtPosition = { 0, 0, 0 };
		Quaternion rotation = Quaternion::Identity();
		real32 rotateSpeed = 0.05f;
		real32 moveSpeed = 0.05f;
		
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
			return lookAtPosition - GetLookVector();
		}
		virtual void SetCameraPosition(const Vector3 &newPosition)
		{
			//TODO(Ian): Make this properly handle camera roll
			Vector3 relative = newPosition - lookAtPosition;
			zoomLevel = relative.Magnitude();
			relative /= zoomLevel;
			if (relative.Dot(Vector3::Up()) > 0.0001f)
			{
				Vector3 axis = relative.Cross(Vector3::Up());
				real32 angle = acosf(relative.y);
				rotation = Quaternion::CreateFromAxisAngle(axis, angle);
			}
			else
			{
				//TODO(Ian): How do we determine the camera roll when it's unreliably close to the upward direction?
				//           Maybe based on the previous camera orientation?
				Assert(false); //Not implemented error
			}
		}

		real32 GetZoom() const
		{
			return zoomLevel;
		}
		void SetZoom(real32 newZoom)
		{
			zoomLevel = newZoom;
		}

		Vector3 GetLookVector() const
		{
			return rotation * Vector3::Forward();
		}

		Matrix4 GetView() const
		{
			Vector3 zoomVec = { 0, 0, -zoomLevel };
			Matrix4 view = rotation.Inverse().ToRotationMatrix() * Matrix4::CreateTranslation(lookAtPosition * -1 + rotation.Transform(zoomVec));
			return view;
		}

		void Rotate(const Quaternion &spin)
		{
			rotation = rotation * spin;
			rotation = rotation.Normalize();
		}

		void HandleInput(const Input::InputManager &manager)
		{
			using Keys = Input::PhysicalInputs;
			Vector3 rotateDirection = {};
			if (manager.IsDown(Keys::S)) //About X axis
			{
				rotateDirection.x++;
			}
			if (manager.IsDown(Keys::W))
			{
				rotateDirection.x--;
			}
			if (manager.IsDown(Keys::A)) //About Y axis
			{
				rotateDirection.y--;
			}
			if (manager.IsDown(Keys::D))
			{
				rotateDirection.y++;
			}
			if (manager.IsDown(Keys::E)) //About Z Axis
			{
				rotateDirection.z--;
			}
			if (manager.IsDown(Keys::Q))
			{
				rotateDirection.z++;
			}

			if (rotateDirection.MagnitudeSquared() != 0)
			{
				rotateDirection = rotateDirection.Normalize();
				Rotate(Quaternion::CreateFromAxisAngle(rotateDirection, rotateSpeed));
			}

			if (manager.IsDown(Keys::R))
			{
				rotation = Quaternion::Identity();
			}

			Vector3 moveDirection = {};
			if (manager.IsDown(Keys::Right))
			{
				moveDirection.x++;
			}
			if (manager.IsDown(Keys::Left))
			{
				moveDirection.x--;
			}
			if (manager.IsDown(Keys::Up))
			{
				moveDirection.z--;
			}
			if (manager.IsDown(Keys::Down))
			{
				moveDirection.z++;
			}
			if (manager.IsDown(Keys::PageUp))
			{
				moveDirection.y++;
			}
			if (manager.IsDown(Keys::PageDown))
			{
				moveDirection.y--;
			}

			if (moveDirection.MagnitudeSquared() != 0)
			{
				moveDirection = moveDirection.Normalize() * moveSpeed;
			}
			lookAtPosition += moveDirection;
		}

		void Update() {}
	};
}

#endif