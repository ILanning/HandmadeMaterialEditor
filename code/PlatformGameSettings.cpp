#ifndef HANDMADE_PLATFORMGAMESETTINGS_CPP
#define HANDMADE_PLATFORMGAMESETTINGS_CPP

#include "PlatformGameSettings.h"
#include "general\StringHelpers.cpp"
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

void PlatformGameSettings::SetWindowTitle(const char *newTitle, const int32 newTitleLength)
{
	delete[] windowTitle;
	windowTitle = CString::CopySubstring(newTitle, newTitleLength);
	windowTitleLength = newTitleLength;
	titleChanged = true;
}

#endif