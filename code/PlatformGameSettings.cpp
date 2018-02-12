#ifndef HANDMADE_PLATFORMGAMESETTINGS_CPP
#define HANDMADE_PLATFORMGAMESETTINGS_CPP

#include "PlatformGameSettings.h"
#include "general\StringHelpers.cpp"

void PlatformGameSettings::SetWindowTitle(const char *newTitle, const int32 newTitleLength)
{
	delete[] windowTitle;
	windowTitle = CString::CopySubstring(newTitle, newTitleLength);
	windowTitleLength = newTitleLength;
	titleChanged = true;
}

#endif