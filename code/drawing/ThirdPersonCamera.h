#ifndef HANDMADE_THIRDPERSONCAMERA
#define HANDMADE_THIRDPERSONCAMERA

#include "../math/Matrix4.h"
#include "../math/Quaternion.h"
#include "../math/Vector3.h"
#include "../input/InputManager.h"

namespace Drawing
{
	struct ThirdPersonCamera
	{
		real32 zoomLevel = 1;
		Vector3 Position = { 0, 0, 0 };
		Quaternion rotation = Quaternion::Identity();
		Matrix4 Projection;
		
		Vector3 GetLookVector()
		{
			return rotation * Vector3::Forward();
		}

		Matrix4 GetView()
		{
			Vector3 zoomVec = { 0, 0, -zoomLevel };
			Matrix4 view = rotation.Inverse().ToRotationMatrix() * Matrix4::CreateTranslation(Position * -1 + rotation.Transform(zoomVec));
			return view;
		}

		Matrix4 GetViewProjection()
		{
			return GetView() * Projection;
		}

		void Rotate(Quaternion spin)
		{
			rotation = rotation * spin;
			rotation = rotation.Normalize();
		}

		void HandleInput(Input::InputManager *manager)
		{
			//TODO(Ian): Use InputManager instead
			using Keys = Input::PhysicalInputs;
			Vector3 rotateDirection = {};
			if (manager->IsDown(Keys::S))
			{
				rotateDirection.x++;
			}
			if (manager->IsDown(Keys::W))
			{
				rotateDirection.x--;
			}
			if (manager->IsDown(Keys::A)) //About Y axis
			{
				rotateDirection.y--;
			}
			if (manager->IsDown(Keys::D))
			{
				rotateDirection.y++;
			}
			if (manager->IsDown(Keys::E)) //About Z Axis
			{
				rotateDirection.z--;
			}
			if (manager->IsDown(Keys::Q))
			{
				rotateDirection.z++;
			}

			real32 rotateSpeed = 0.05f;
			if (rotateDirection.MagnitudeSquared() != 0)
			{
				rotateDirection = rotateDirection.Normalize();
				Rotate(Quaternion::CreateFromAxisAngle(rotateDirection, rotateSpeed));
			}

			if (manager->IsDown(Keys::R))
			{
				rotation = Quaternion::Identity();
			}

			Vector3 moveDirection = {};
			if (manager->IsDown(Keys::Right))
			{
				moveDirection.x++;
			}
			if (manager->IsDown(Keys::Left))
			{
				moveDirection.x--;
			}
			if (manager->IsDown(Keys::Up))
			{
				moveDirection.z--;
			}
			if (manager->IsDown(Keys::Down))
			{
				moveDirection.z++;
			}
			if (manager->IsDown(Keys::PageUp))
			{
				moveDirection.y++;
			}
			if (manager->IsDown(Keys::PageDown))
			{
				moveDirection.y--;
			}

			real32 moveSpeed = 0.03f;
			if (moveDirection.MagnitudeSquared() != 0)
			{
				moveDirection = moveDirection.Normalize() * moveSpeed;
			}
			Position += moveDirection;
		}
	};
}

#endif