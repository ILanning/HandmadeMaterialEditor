#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define WIN32_HANDMADE_TEST
#include "../libraries/doctest.h"
#include <iostream>
#include "test_helpers.cpp"

TEST_SUITE_BEGIN("math");

#include "math/test_vector2.cpp"
#include "math/test_vector3.cpp"

TEST_SUITE_END();

//-lts list test suites
//-tc=*x*
//-sf --source-file=
//-ts --test-suite=