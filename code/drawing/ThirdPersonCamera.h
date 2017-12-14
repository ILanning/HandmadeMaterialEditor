#ifndef HANDMADE_THIRDPERSONCAMERA
#define HANDMADE_THIRDPERSONCAMERA

#include "../math/Matrix4.h"
#include "../math/Quaternion.h"
#include "../math/Vector3.h"

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

		void HandleInput(game_input *input)
		{
			//TODO(Ian): Use InputManager instead
			Vector3 rotateDirection = {};
			if (input->Controllers[0].MoveDown.EndedDown)
			{
				rotateDirection.x++;
			}
			if (input->Controllers[0].MoveUp.EndedDown)
			{
				rotateDirection.x--;
			}
			if (input->Controllers[0].MoveLeft.EndedDown) //About Y axis
			{
				rotateDirection.y--;
			}
			if (input->Controllers[0].MoveRight.EndedDown)
			{
				rotateDirection.y++;
			}
			if (input->Controllers[0].RightShoulder.EndedDown) //About Z Axis
			{
				rotateDirection.z--;
			}
			if (input->Controllers[0].LeftShoulder.EndedDown)
			{
				rotateDirection.z++;
			}

			real32 rotateSpeed = 0.05f;
			if (rotateDirection.MagnitudeSquared() != 0)
			{
				rotateDirection = rotateDirection.Normalize();
				Rotate(Quaternion::CreateFromAxisAngle(rotateDirection, rotateSpeed));
			}

			Vector3 moveDirection = {};
			if (input->Controllers[0].ActionRight.EndedDown)
			{
				moveDirection.x++;
			}
			if (input->Controllers[0].ActionLeft.EndedDown)
			{
				moveDirection.x--;
			}
			if (input->Controllers[0].ActionUp.EndedDown)
			{
				moveDirection.z--;
			}
			if (input->Controllers[0].ActionDown.EndedDown)
			{
				moveDirection.z++;
			}
			if (input->Controllers[0].Start.EndedDown)
			{
				moveDirection.y++;
			}
			if (input->Controllers[0].Back.EndedDown)
			{
				moveDirection.y--;
			}

			real32 moveSpeed = 0.03f;
			if (rotateDirection.MagnitudeSquared() != 0)
			{
				moveDirection = moveDirection.Normalize() * moveSpeed;
			}
			Position += moveDirection;
		}
	};
}

#endif