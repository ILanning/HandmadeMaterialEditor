#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define WIN32_HANDMADE_TEST

#include "../libraries/doctest.h"
#include <iostream>
#include "test_helpers.h"

TEST_SUITE_BEGIN("math");

#include "math/test_vector3.cpp"
#include "math/test_vector2.cpp"
#include "math/test_matrix3.cpp"
#include "math/test_matrix4.cpp"
#include "math/test_quaternion.cpp"

TEST_SUITE_END();

TEST_SUITE_BEGIN("general");

#include "general/test_stringhelpers.cpp"
#include "general/test_pathhelpers.cpp"
#include "general/test_stackarena.cpp"
#include "general/test_fixedsizearena.cpp"
#include "general/test_arraylist.cpp"
#include "general/test_hashmap.cpp"

TEST_SUITE_END();

TEST_SUITE_BEGIN("file");

#include "file/test_pairserializer.cpp"

TEST_SUITE_END();

TEST_SUITE_BEGIN("content");

#include "content/test_assetmanager.cpp"

TEST_SUITE_END();

TEST_SUITE_BEGIN("unsorted");

#include "test_platformgamesettings.cpp"

TEST_SUITE_END();

#ifdef HANDMADE_UNITY

#include "../content/AssetManager.cpp"
#include "../content/MTLLoader.cpp"
#include "../content/OBJLoader.cpp"
#include "../content/ShaderManager.cpp"

#include "../drawing/defaults/DefaultMaterials.cpp"
#include "../drawing/Texture2D.cpp"
#include "../drawing/VertexColorTextureArray.cpp"
#include "../drawing/VertexNormalTextureArray.cpp"

#include "../general/HashMap.cpp"
#include "../general/PathHelpers.cpp"

#include "../math/Matrix3.cpp"
#include "../math/Matrix4.cpp"
#include "../math/Quaternion.cpp"
#include "../math/Vector2.cpp"
#include "../math/Vector3.cpp"

#include "../PlatformGameSettings.cpp"
#include "../library_implementations.cpp"

#endif
//-lts list test suites
//-tc=*x*
//-sf --source-file=
//-ts --test-suite=