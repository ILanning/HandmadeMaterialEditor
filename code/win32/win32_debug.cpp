#ifndef HANDMADE_WIN32_DEBUG
#define HANDMADE_WIN32_DEBUG

#include "../handmade_typedefs.h"

#include <windows.h>
#include <stdio.h>
#include <malloc.h>

DEBUG_PLATFORM_FREE_FILE_MEMORY(DEBUGPlatformFreeFileMemory)
{
	if (Memory)
	{
		VirtualFree(Memory, 0, MEM_RELEASE);
	}
}

DEBUG_PLATFORM_READ_ENTIRE_FILE(DEBUGPlatformReadEntireFile)
{
	debug_read_file_result Result = {};
	bool success = false;

	HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER FileSize;
		if (GetFileSizeEx(FileHandle, &FileSize))
		{
			uint32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
			Result.Contents = VirtualAlloc(0, FileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (Result.Contents)
			{
				DWORD BytesRead;
				if (ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) &&
					(FileSize32 == BytesRead))
				{
					// NOTE(casey): File read successfully
					success = true;
					Result.ContentsSize = FileSize32;
				}
				else
				{
					// TODO(casey): Logging
					DEBUGPlatformFreeFileMemory(Thread, Result.Contents);
					Result.Contents = 0;
				}
			}
			else
			{
				// TODO(casey): Logging
			}
		}
		else
		{
			// TODO(casey): Logging
		}

		CloseHandle(FileHandle);
	}
	else
	{
		// TODO(casey): Logging
	}

	if (outSuccess)
	{
		*outSuccess = success;
	}

	return(Result);
}

PLATFORM_READ_FILE(DebugReadWrapper)
{
	FileData result = {};
	result.Path = path;
	result.PathSize = pathLength;

	bool success = false;
	debug_read_file_result file = DEBUGPlatformReadEntireFile({}, path, &success);

	if (success)
	{
		result.File = file.Contents;
		result.FileSize = file.ContentsSize;
		result.IsLoaded = true;
	}
	else
	{
		result.IsLoaded = false;
	}
	if (outSuccess)
	{
		*outSuccess = success;
	}

	return result;
}

DEBUG_PLATFORM_MESSAGE_ERROR_FUNC(VSOutputDebugString)
{
	OutputDebugStringA(errorString);
}

DEBUG_PLATFORM_WRITE_ENTIRE_FILE(DEBUGPlatformWriteEntireFile)
{
	bool32 Result = false;

	HANDLE FileHandle = CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD BytesWritten;
		if (WriteFile(FileHandle, Memory, MemorySize, &BytesWritten, 0))
		{
			// NOTE(casey): File read successfully
			Result = (BytesWritten == MemorySize);
		}
		else
		{
			// TODO(casey): Logging
		}

		CloseHandle(FileHandle);
	}
	else
	{
		// TODO(casey): Logging
	}

	return(Result);
}

#endif