#ifndef HANDMADE_PLATFORMGAMESETTINGS_CPP
#define HANDMADE_PLATFORMGAMESETTINGS_CPP

#include "PlatformGameSettings.h"
#include "file\PairSerializer.h"
#include "general\HashMap.h"
#include "general\StringHelpers.h"
#include "math\Vector2.h"
#include <cstdlib>

void PlatformGameSettings::Swap(PlatformGameSettings &other)
{
	char tempBuffer[sizeof(PlatformGameSettings)];
	memcpy(tempBuffer, this, sizeof(PlatformGameSettings));
	memcpy(this, &other, sizeof(PlatformGameSettings));
	memcpy(&other, tempBuffer, sizeof(PlatformGameSettings));
}

PlatformGameSettings::PlatformGameSettings(const PlatformGameSettings &other)
{
	memcpy(this, &other, sizeof(PlatformGameSettings));
	windowTitle = CString::CopySubstring(other.windowTitle, other.windowTitleLength);
}  

bool PlatformGameSettings::Import(HMString filePath, ReadFileFunc& readFile, Memory::NewDeleteArena& allocator)
{
	bool success = false;
	Collections::HashMap<HMString, HMString, Memory::NewDeleteArena> dict = 
		File::PairSerializer::ParseFile(filePath, readFile, allocator, &success);

	if (!success)
	{
		return false;
	}
	
	if (dict.CheckExists({ "WindowedXPosition" }))
	{
		HMString string = dict[{ "WindowedXPosition" }];
		int32 result = 0;
		if (CString::TryParseInt32(string.RawCString(), string.Length(), result))
		{
			CachedWindowedPosition.x = (real32)result;
		}
	}
	if (dict.CheckExists({ "WindowedYPosition" }))
	{
		HMString string = dict[{ "WindowedYPosition" }];
		int32 result = 0;
		if (CString::TryParseInt32(string.RawCString(), string.Length(), result))
		{
			CachedWindowedPosition.y = (real32)result;
		}
	}

	if (dict.CheckExists({ "WindowedXSize" }))
	{
		HMString string = dict[{ "WindowedXSize" }];
		int32 result = 0;
		if (CString::TryParseInt32(string.RawCString(), string.Length(), result))
		{
			CachedWindowedSize.x = (real32)result;
		}
	}
	if (dict.CheckExists({ "WindowedYSize" }))
	{
		HMString string = dict[{ "WindowedYSize" }];
		int32 result = 0;
		if (CString::TryParseInt32(string.RawCString(), string.Length(), result))
		{
			CachedWindowedSize.y = (real32)result;
		}
	}

	if (dict.CheckExists({ "WindowTitle" }))
	{
		HMString string = dict[{ "WindowTitle" }];
		windowTitle = CString::CopySubstring(string.RawCString(), string.Length() - 1, &windowTitleLength);
	}

	if (dict.CheckExists({ "Fullscreen" }))
	{
		HMString string = dict[{ "Fullscreen" }];
		bool result = false;
		if (CString::TryParseBool(string.RawCString(), string.Length(), result))
		{
			Fullscreen = result;
		}
	}

	return true;
}

bool PlatformGameSettings::Export(HMString filePath, WriteFileFunc* writeFile) const
{
	const int32 bufferSize = 300;
	char buffer[bufferSize];
	int32 writeHead = 0;
	HMString text = { "WindowedXPosition" };
	HMString spacer = { " = " };
	HMString newLine = { "\r\n" };
	writeHead = text.CopyTo(buffer, bufferSize, writeHead);
	writeHead = spacer.CopyTo(buffer, bufferSize, writeHead);
	writeHead += CString::WriteInt(buffer + writeHead, bufferSize, (int32)CachedWindowedPosition.x, MaxInt32);
	writeHead = newLine.CopyTo(buffer, bufferSize, writeHead);

	text = { "WindowedYPosition" };
	writeHead = text.CopyTo(buffer, bufferSize, writeHead);
	writeHead = spacer.CopyTo(buffer, bufferSize, writeHead);
	writeHead += CString::WriteInt(buffer + writeHead, bufferSize, (int32)CachedWindowedPosition.y, MaxInt32);
	writeHead = newLine.CopyTo(buffer, bufferSize, writeHead);

	text = { "WindowedXSize" };
	writeHead = text.CopyTo(buffer, bufferSize, writeHead);
	writeHead = spacer.CopyTo(buffer, bufferSize, writeHead);
	writeHead += CString::WriteInt(buffer + writeHead, bufferSize, (int32)CachedWindowedSize.x, MaxInt32);
	writeHead = newLine.CopyTo(buffer, bufferSize, writeHead);

	text = { "WindowedYSize" };
	writeHead = text.CopyTo(buffer, bufferSize, writeHead);
	writeHead = spacer.CopyTo(buffer, bufferSize, writeHead);
	writeHead += CString::WriteInt(buffer + writeHead, bufferSize, (int32)CachedWindowedSize.y, MaxInt32);
	writeHead = newLine.CopyTo(buffer, bufferSize, writeHead);

	text = { "WindowTitle" };
	writeHead = text.CopyTo(buffer, bufferSize, writeHead);
	writeHead = spacer.CopyTo(buffer, bufferSize, writeHead);
	buffer[writeHead++] = '"';
	HMString title = { windowTitle, (uint32)windowTitleLength };
	writeHead = title.CopyTo(buffer, bufferSize, writeHead);
	buffer[writeHead++] = '"';
	writeHead = newLine.CopyTo(buffer, bufferSize, writeHead);

	text = { "Fullscreen" };
	writeHead = text.CopyTo(buffer, bufferSize, writeHead);
	writeHead = spacer.CopyTo(buffer, bufferSize, writeHead);
	writeHead += CString::WriteBool(buffer + writeHead, bufferSize, Fullscreen);
	buffer[writeHead++] = '\0';

	bool success = false;
	writeFile(filePath.RawCString(), filePath.Length(), buffer, writeHead, &success);
	return success;
}

void PlatformGameSettings::SetWindowTitle(const char *newTitle, const int32 newTitleLength)
{
	delete[] windowTitle;
	windowTitle = CString::CopySubstring(newTitle, newTitleLength);
	windowTitleLength = newTitleLength;
	titleChanged = true;
}

#endif