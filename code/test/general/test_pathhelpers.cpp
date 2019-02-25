#ifndef HANDMADE_TEST_PATHHELPERS
#define HANDMADE_TEST_PATHHELPERS

#include "../../handmade_typedefs.h"
#include "../../general/PathHelpers.h"

TEST_CASE("Testing various path manipulation functions")
{
	const char *relativeFile = "/test/bin/Windows Platform/executable.exe";
	const char *relativeFolder = "code/general";
	const char *relativeIsland = "/island//";

	const char *absoluteFile = "E:/Games/Handmade Material Editor/bin/win32_handmade.exe";
	const char *absoluteFolder = "C:/Program Files (x86)/folder/";

	const char *drive = "D:";
	const char *emptyString = "";

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
		const char *resultA = "code/general/test/bin/Windows Platform/executable.exe";
		const char *resultB = "C:/Program Files (x86)/folder/test/bin/Windows Platform/executable.exe";

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
		const char *resultA = "/test/bin/Windows Platform/";
		const char *resultB = "C:/Program Files (x86)/";
		const char *resultC = "/";
		const char *resultD = "";

		int32 length = 0;
		char *name = Path::CloneParentDirectory(relativeFile, &length);
		CHECK(CString::IsEqual(name, resultA));
		CHECK(length == 28);
		delete[] name;
		name = Path::CloneParentDirectory(absoluteFolder);
		CHECK(CString::IsEqual(name, resultB));
		delete[] name;
		name = Path::CloneParentDirectory(relativeIsland);
		CHECK(CString::IsEqual(name, resultC));
		delete[] name;
		name = Path::CloneParentDirectory(drive);
		CHECK(CString::IsEqual(name, resultD));
		delete[] name;
	}
	SUBCASE("Testing GetEndName")
	{
		const char *resultA = "executable.exe";
		const char *resultB = "folder";
		const char *resultC = "island";
		const char *resultD = "D:";

		int32 length = 0;
		char *name = Path::CloneEndName(relativeFile, &length);
		CHECK(CString::IsEqual(name, resultA));
		CHECK(length == 15);
		delete[] name;
		name = Path::CloneEndName(absoluteFolder);
		CHECK(CString::IsEqual(name, resultB));
		delete[] name;
		name = Path::CloneEndName(relativeIsland);
		CHECK(CString::IsEqual(name, resultC));
		delete[] name;
		name = Path::CloneEndName(drive);
		CHECK(CString::IsEqual(name, resultD));
		delete[] name;
	}
}

#endif