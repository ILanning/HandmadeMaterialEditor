#ifndef HANDMADE_TEST_THIRDPERSONCAMERA
#define HANDMADE_TEST_THIRDPERSONCAMERA

#include "../../handmade_typedefs.h"
#include "../../math/Quaternion.h"
#include "../../math/Vector3.h"
#include "../../math/Matrix4.h"
#include "../../drawing/ThirdPersonCamera.h"
#include <iostream>

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../libraries/doctest.h"
#endif

TEST_CASE("Testing the third person camera class")
{
	//CHECK();
	SUBCASE("Test")
	{
		Drawing::ThirdPersonCamera camera = {};
		camera.zoomLevel = 10;
		Vector3 rotateDirection = { 1, 1, 0 };
		real32 rotateSpeed = 0.5f;

		rotateDirection = rotateDirection.Normalize();
		Vector3 rotateAxis = camera.GetLookVector().Cross(rotateDirection).Normalize();
		camera.rotation = Quaternion::CreateFromAxisAngle(rotateAxis, rotateSpeed) * camera.rotation;

		std::cout << camera.GetView() << '\n';
	}
}

#endif