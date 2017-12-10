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
	};
}

#endif