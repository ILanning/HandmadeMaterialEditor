#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define WIN32_HANDMADE_TEST

#include "../libraries/doctest.h"
#include <iostream>
#include "test_helpers.cpp"

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

#include "file/test_pairtokenizer.cpp"

TEST_SUITE_END();

//-lts list test suites
//-tc=*x*
//-sf --source-file=
//-ts --test-suite=