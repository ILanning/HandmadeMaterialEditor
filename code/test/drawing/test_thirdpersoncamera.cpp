#ifndef HANDMADE_TEST_THIRDPERSONCAMERA_CPP
#define HANDMADE_TEST_THIRDPERSONCAMERA_CPP

#include "../../handmade_typedefs.h"
#include "../../math/Quaternion.h"
#include "../../math/Vector3.h"
#include "../../math/Matrix4.h"
#include "../../drawing/ThirdPersonCamera.h"
#include <iostream>

TEST_CASE("Testing the third person camera class")
{
	//CHECK();
	SUBCASE("Test")
	{
		Drawing::ThirdPersonCamera camera = {};
		camera.rotation = Quaternion::CreateFromAxisAngle(Vector3::Up(), Pi32 / 8);
		std::cout << camera.rotation << '\n';
		std::cout << camera.rotation.Transform({ 0, 0, 10 });
		std::cout << camera.GetView() << '\n';
	}
}

#endif //HANDMADE_TEST_THIRDPERSONCAMERA_CPP