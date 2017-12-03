#ifndef HANDMADE_TEST_PATHHELPERS
#define HANDMADE_TEST_PATHHELPERS

#include "../../handmade_typedefs.h"
#include "../../general/PathHelpers.cpp"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../libraries/doctest.h"
#endif

TEST_CASE("Testing various path manipulation functions")
{
	char *relativeFile = "/test/bin/Windows Platform/executable.exe";
	char *relativeFolder = "code/general";
	char *relativeIsland = "/island/";

	char *absoluteFile = "E:/Games/Handmade Material Editor/bin/win32_handmade.exe";
	char *absoluteFolder = "C:/Program Files (x86)/folder/";

	char *drive = "D:";
	char *emptyString = "";

	SUBCASE("Testing IsAbsolute")
	{
		CHECK(!Path::IsAbsolute(relativeFile));
		CHECK(!Path::IsAbsolute(relativeFolder));
		CHECK(!Path::IsAbsolute(emptyString));

		CHECK(Path::IsAbsolute(absoluteFile));
		CHECK(Path::IsAbsolute(absoluteFolder));
		CHECK(Path::IsAbsolute(drive));
	}
	SUBCASE("Testing IsRelative")
	{
		CHECK(Path::IsRelative(relativeFile));
		CHECK(Path::IsRelative(relativeFolder));
		CHECK(Path::IsRelative(emptyString));

		CHECK(!Path::IsRelative(absoluteFile));
		CHECK(!Path::IsRelative(absoluteFolder));
		CHECK(!Path::IsRelative(drive));
	}
	SUBCASE("Testing IsPathSeperator")
	{
		CHECK(Path::IsPathSeperator('\\'));
		CHECK(Path::IsPathSeperator('/'));
		CHECK(!Path::IsPathSeperator('P'));
	}
	SUBCASE("Testing Combine")
	{
		char *resultA = "code/general/test/bin/Windows Platform/executable.exe";
		char *resultB = "C:/Program Files (x86)/folder/test/bin/Windows Platform/executable.exe";

		char *combined = Path::Combine(relativeFolder, relativeFile);
		CHECK(CString::IsEqual(combined, resultA));
		delete[] combined;
		combined = Path::Combine(relativeFolder, relativeFile + 1);
		CHECK(CString::IsEqual(combined, resultA));
		delete[] combined;
		combined = Path::Combine(absoluteFolder, relativeFile);
		CHECK(CString::IsEqual(combined, resultB));
		delete[] combined;
		combined = Path::Combine(absoluteFolder, relativeFile + 1);
		CHECK(CString::IsEqual(combined, resultB));
		delete[] combined;
	}
	SUBCASE("Testing GetParentDirectory")
	{
		char *resultA = "/test/bin/Windows Platform/";
		char *resultB = "C:/Program Files (x86)/";
		char *resultC = "/";
		char *resultD = "";

		int32 length = 0;
		char *name = Path::GetParentDirectory(relativeFile, &length);
		CHECK(CString::IsEqual(name, resultA));
		CHECK(length == 28);
		delete[] name;
		name = Path::GetParentDirectory(absoluteFolder);
		CHECK(CString::IsEqual(name, resultB));
		delete[] name;
		name = Path::GetParentDirectory(relativeIsland);
		CHECK(CString::IsEqual(name, resultC));
		delete[] name;
		name = Path::GetParentDirectory(drive);
		CHECK(CString::IsEqual(name, resultD));
		delete[] name;
	}
	SUBCASE("Testing GetEndName")
	{
		char *resultA = "executable.exe";
		char *resultB = "folder";
		char *resultC = "island";
		char *resultD = "D:";

		int32 length = 0;
		char *name = Path::GetEndName(relativeFile, &length);
		CHECK(CString::IsEqual(name, resultA));
		CHECK(length == 15);
		delete[] name;
		name = Path::GetEndName(absoluteFolder);
		CHECK(CString::IsEqual(name, resultB));
		delete[] name;
		name = Path::GetEndName(relativeIsland);
		CHECK(CString::IsEqual(name, resultC));
		delete[] name;
		name = Path::GetEndName(drive);
		CHECK(CString::IsEqual(name, resultD));
		delete[] name;
	}
}

#endif