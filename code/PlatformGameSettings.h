#ifndef HANDMADE_PLATFORMGAMESETTINGS_H
#define HANDMADE_PLATFORMGAMESETTINGS_H

#include "handmade_typedefs.h"
#include "handmade_funcdefs.h"
#include "math\Vector2.h"
#include "general\HMString.h"
#include "general\memory\NewDeleteArena.h"

struct PlatformGameSettings
{
private:
	char *windowTitle = nullptr;
	int32 windowTitleLength = 0;

public:
	bool titleChanged = false;

	/** The current location of the window's top left corner.
	*/
	Vector2 WindowPosition = { 0, 0 };
	/** If in fullscreen, the previous position of the window's top left corner.
	*/
	Vector2 CachedWindowedPosition = { 0, 0 };

	/** The pixel dimensions of the window.
	*/
	Vector2 WindowSize = { 0, 0 };
	/** If in fullscreen, the previous size of the window.
	*/
	Vector2 CachedWindowedSize = { 0, 0 };

	/** Whether or not the game is in fullscreen mode or not.
	*/
	bool Fullscreen = false;

	/** Whether or not the mouse is visible.
	*/
	bool MouseVisible = true;

	/** Whether or not the window is in focus.
	*/
	bool Focused = false;

	const char* DefaultFilePath = "settings.txt";

	void Swap(PlatformGameSettings &other);

	inline PlatformGameSettings() {};

	PlatformGameSettings(const PlatformGameSettings &other);

	inline PlatformGameSettings(HMString filePath, ReadFileFunc& readFile, Memory::NewDeleteArena& scratchArena) : PlatformGameSettings()
	{
		Import(filePath, readFile, scratchArena);
	}

	inline PlatformGameSettings& operator=(PlatformGameSettings arg)
	{
		Swap(arg);
		return *this;
	}

	inline bool operator==(const PlatformGameSettings& other) const
	{
		if (titleChanged == other.titleChanged &&
			Fullscreen == other.Fullscreen &&
			MouseVisible == other.MouseVisible &&
			windowTitleLength == other.windowTitleLength &&
			WindowSize == other.WindowSize &&
			CachedWindowedSize == other.CachedWindowedSize &&
			WindowPosition == other.WindowPosition &&
			CachedWindowedPosition == other.CachedWindowedPosition)
		{
			return true;
		}
		return false;
	}

	inline bool operator!=(const PlatformGameSettings& other) const
	{
		return !(*this == other);
	}

	/** Returns the string that it displayed in the title bar of the window.
	*/
	inline char *GetWindowTitle() const
	{
		return windowTitle;
	}

	/** Sets the title of the window.  Copies input string into personally owned buffer.
	*/
	void SetWindowTitle(const char *newTitle, const int32 newTitleLength);

	/** Imports a settings set from a file.  Returns false if it could not read from the file.
	*/
	bool Import(HMString filePath, ReadFileFunc& readFile, Memory::NewDeleteArena& allocator);

	/** Writes the current settings to a file, overwriting the contents.  Returns false if it could not write to or create the file.
	*/
	bool Export(HMString filePath, WriteFileFunc* writeFile) const;

	/** Creates a basic default settings string.
	*/
	inline static HMString GenDefault()
	{
		return {
"WindowedXPosition = 40\r\n\
WindowedYPosition = 40\r\n\
WindowedXSize = 1280\r\n\
WindowedYSize = 720\r\n\
WindowTitle = \"Handmade Material Editor:Virt Test\"\r\n\
Fullscreen = false"
		};
	}

	inline ~PlatformGameSettings()
	{
		delete[] windowTitle;
	}
};

#define PLATFORM_UPDATE_SETTINGS(name) void name(PlatformGameSettings settings)
typedef PLATFORM_UPDATE_SETTINGS(PlatformUpdateSettingsFunc);

#endif