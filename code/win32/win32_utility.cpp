internal void
CatStrings(size_t SourceACount, char *SourceA,
	size_t SourceBCount, char *SourceB,
	size_t DestCount, char *Dest)
{
	// TODO(casey): Dest bounds checking!

	for (int Index = 0;
		Index < SourceACount;
		++Index)
	{
		*Dest++ = *SourceA++;
	}

	for (int Index = 0;
		Index < SourceBCount;
		++Index)
	{
		*Dest++ = *SourceB++;
	}

	*Dest++ = 0;
}

internal int
StringLength(char *String)
{
	int Count = 0;
	while (*String++)
	{
		++Count;
	}
	return(Count);
}

internal void
Win32GetEXEFileName(win32_state *State)
{
	// NOTE(casey): Never use MAX_PATH in code that is user-facing, because it
	// can be dangerous and lead to bad results.
	DWORD SizeOfFilename = GetModuleFileNameA(0, State->EXEFileName, sizeof(State->EXEFileName));
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

internal void
Win32BuildEXEPathFileName(win32_state *State, char *FileName,
	int DestCount, char *Dest)
{
	CatStrings(State->OnePastLastEXEFileNameSlash - State->EXEFileName, State->EXEFileName,
		StringLength(FileName), FileName,
		DestCount, Dest);
}

internal win32_window_dimension
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