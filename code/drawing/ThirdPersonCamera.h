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
			Matrix4 view = Matrix4::CreateTranslation(Position) * rotation.ToRotationMatrix() * Matrix4::CreateScale(zoomLevel, zoomLevel, zoomLevel);
			return view;
		}

		Matrix4 GetViewProjection()
		{
			return GetView() * Projection;
		}

		void Rotate(Quaternion spin)
		{
			rotation = spin * rotation;
			rotation = rotation.Normalize();
		}
	};
}

#endif