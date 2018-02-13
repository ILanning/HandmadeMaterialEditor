#ifndef HANDMADE_PLATFORMGAMESETTINGS_H
#define HANDMADE_PLATFORMGAMESETTINGS_H

#include "general\StringHelpers.cpp"
#include "handmade_typedefs.h"
#include "math\Vector2.h"

struct PlatformGameSettings
{
private:
	char *windowTitle = nullptr;
	int32 windowTitleLength = 0;
	bool titleChanged = false;

public:
	Vector2 WindowPosition = { 0, 0 };
	Vector2 cachedPosition = { 0, 0 };
	Vector2 WindowSize = { 0, 0 };
	Vector2 cachedSize = { 0, 0 };
	bool Fullscreen = false;
	bool MouseVisible = true; 
	bool Focused = false;
	
	void Swap(PlatformGameSettings &other)
	{
		char tempBuffer[sizeof(PlatformGameSettings)];
		memcpy(tempBuffer, this, sizeof(PlatformGameSettings));
		memcpy(this, &other, sizeof(PlatformGameSettings));
		memcpy(&other, tempBuffer, sizeof(PlatformGameSettings));
	}

	PlatformGameSettings() {};

	PlatformGameSettings(const PlatformGameSettings &other)
	{
		memcpy(this, &other, sizeof(PlatformGameSettings));
		windowTitle = CString::CopySubstring(other.windowTitle, other.windowTitleLength);
	}

	PlatformGameSettings& operator=(PlatformGameSettings arg)
	{
		Swap(arg);
		return *this;
	}

	bool operator==(const PlatformGameSettings& other) const
	{
		if (titleChanged == other.titleChanged &&
			Fullscreen == other.Fullscreen &&
			MouseVisible == other.MouseVisible &&
			windowTitleLength == other.windowTitleLength &&
			WindowSize == other.WindowSize &&
			WindowPosition == other.WindowPosition)
		{
			return true;
		}
		return false;
	}

	bool operator!=(const PlatformGameSettings& other) const
	{
		return !(*this == other);
	}

	char *GetWindowTitle() const
	{
		return windowTitle;
	}

	/** Sets the title of the window.  Copies input string into personally owned buffer.
	*/
	void SetWindowTitle(const char *newTitle, const int32 newTitleLength);

	~PlatformGameSettings()
	{
		delete[] windowTitle;
	}
};

#endif