#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define WIN32_HANDMADE_TEST
#include "../libraries/doctest.h"
#include <iostream>
#include "test_helpers.cpp"

TEST_SUITE_BEGIN("math");

#include "math/test_vector2.cpp"
#include "math/test_vector3.cpp"
#include "math/test_matrix3.cpp"
#include "math/test_matrix4.cpp"
#include "math/test_quaternion.cpp"

TEST_SUITE_END();

TEST_SUITE_BEGIN("general");

#include "general/test_stringhelpers.cpp"
#include "general/test_pathhelpers.cpp"
#include "general/test_stackarena.cpp"
#include "general/test_fixedsizearena.cpp"

TEST_SUITE_END();

//#include "drawing/test_thirdpersoncamera.cpp"

//-lts list test suites
//-tc=*x*
//-sf --source-file=
//-ts --test-suite=