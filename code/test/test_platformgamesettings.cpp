#ifndef HANDMADE_TEST_PLATFORMGAMESETTINGS
#define HANDMADE_TEST_PLATFORMGAMESETTINGS

#include "../handmade_funcdefs.h"
#include "../PlatformGameSettings.h"
#include "../general/HashMap.h"
#include "../general/HMString.h"
#include "../file/FileData.h"
#include "../general/StringHelpers.h"
#include "../general/memory/NewDeleteArena.h"

namespace TestPlatformGameSettingsHelpers
{
	static FileData ReadStash = {};
	static bool ReadSuccess = true;

	static HMString WriteStash = {};
	static bool WriteSuccess = true;

	PLATFORM_READ_FILE(DummyReadFile)
	{
		if (outSuccess)
		{
			*outSuccess = ReadSuccess;
		}
		ReadStash.Path = CString::CopySubstring(path, CString::GetLength(path));
		ReadStash.PathSize = pathLength;
		return ReadStash;
	}

	PLATFORM_WRITE_FILE(DummyWriteFile)
	{
		if (outSuccess)
		{
			*outSuccess = WriteSuccess;
		}
		WriteStash = { CString::CopySubstring((char*)file, fileLength), (uint32)fileLength };
	}
}

TEST_CASE("PlatformGameSettings")
{
	using namespace TestPlatformGameSettingsHelpers;
	Memory::NewDeleteArena arena = {};

	SUBCASE("Testing Import")
	{
		auto expected = PlatformGameSettings();
		auto result = PlatformGameSettings();
		bool success = true;

		HMString defaultText = PlatformGameSettings::GenDefault();
		ReadStash = { true, 0, nullptr, defaultText.Length(), defaultText.RawCString() };
		expected.CachedWindowedPosition = { 40, 40 };
		expected.CachedWindowedSize = { 1280, 720 };
		expected.Fullscreen = false;
		expected.SetWindowTitle("Handmade Material Editor:Virt Test", 35);
		expected.titleChanged = false;
		success = result.Import({ "dummy", 6 }, DummyReadFile, arena);

		CHECK(success == true);
		CHECK(result == expected);

		HMString emptyString = {};
		ReadStash = { true, 0, nullptr, emptyString.Length(), emptyString.RawCString() };
		success = result.Import({"dummy", 6}, DummyReadFile, arena);

		CHECK(success == true);
		CHECK(result == expected);
	}

	SUBCASE("Testing Export")
	{
		auto writeSettings = PlatformGameSettings();
		writeSettings.CachedWindowedPosition = { 40, 40 };
		writeSettings.CachedWindowedSize = { 1280, 720 };
		writeSettings.Fullscreen = false;
		writeSettings.SetWindowTitle("Handmade Material Editor:Virt Test", 35);
		writeSettings.titleChanged = false;

		bool result = writeSettings.Export({"dummy", 6}, DummyWriteFile);
		CHECK(result);
		CHECK(WriteStash == PlatformGameSettings::GenDefault());
	}
}

#endif //HANDMADE_TEST_PLATFORMGAMESETTINGS