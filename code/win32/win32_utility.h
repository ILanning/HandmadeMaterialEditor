#ifndef WIN32_UTILITY_CPP
#define WIN32_UTILITY_CPP

#include "../handmade_typedefs.h"
#include "win32_handmade.h"
#include "../general/StringHelpers.h"
#include <windows.h>

inline FILETIME
Win32GetLastWriteTime(char *Filename)
{
	FILETIME LastWriteTime = {};

	WIN32_FILE_ATTRIBUTE_DATA Data;
	if (GetFileAttributesEx(Filename, GetFileExInfoStandard, &Data))
	{
		LastWriteTime = Data.ftLastWriteTime;
	}

	return(LastWriteTime);
}

inline internal void
Win32GetEXEFileName(win32_state *State)
{
	// NOTE(casey): Never use MAX_PATH in code that is user-facing, because it
	// can be dangerous and lead to bad results.
	State->EXEFileNameSize = GetModuleFileNameA(0, State->EXEFileName, sizeof(State->EXEFileName));
	State->OnePastLastEXEFileNameSlash = State->EXEFileName;
	for (char *Scan = State->EXEFileName;
		*Scan;
		++Scan)
	{
		if (*Scan == '\\')
		{
			State->OnePastLastEXEFileNameSlash = Scan + 1;
		}
	}
}

inline internal void
Win32BuildEXEPathFileName(win32_state *State, const char *FileName,
	int32 DestCount, char *Dest)
{
	CString::CombineStringsToDest(State->EXEFileName, (int32)(State->OnePastLastEXEFileNameSlash - State->EXEFileName), 
								  FileName, CString::GetLength(FileName), 
								  Dest, DestCount);
}

inline internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Width = ClientRect.right - ClientRect.left;
	Result.Height = ClientRect.bottom - ClientRect.top;

	return(Result);
}

inline LARGE_INTEGER
Win32GetWallClock(void)
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return(Result);
}

inline real32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	real32 Result = ((real32)(End.QuadPart - Start.QuadPart) /
		(real32)GlobalPerfCountFrequency);
	return(Result);
}

#endif //WIN32_UTILITY_CPP