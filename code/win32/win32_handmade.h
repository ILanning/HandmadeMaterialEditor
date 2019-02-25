#if !defined(WIN32_HANDMADE_H)
#define WIN32_HANDMADE_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

#include "InputProcessor.h"
#include "../PlatformGameSettings.h"
#include "../general/HMString.h"
#include "../handmade.h"

struct win32_offscreen_buffer
{
    // NOTE(casey): Pixels are alwasy 32-bits wide, Memory Order BB GG RR XX
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct Win32BoundsRect
{
	int32 x;
	int32 y;
	int32 width;
	int32 height;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

struct win32_sound_output
{
    int SamplesPerSecond;
    uint32 RunningSampleIndex;
    int BytesPerSample;
    DWORD SecondaryBufferSize;
    DWORD SafetyBytes;
    real32 tSine;
    // TODO(casey): Should running sample index be in bytes as well
    // TODO(casey): Math gets simpler if we add a "bytes per second" field?
};

struct win32_debug_time_marker
{
    DWORD OutputPlayCursor;
    DWORD OutputWriteCursor;
    DWORD OutputLocation;
    DWORD OutputByteCount;
    DWORD ExpectedFlipPlayCursor;
    
    DWORD FlipPlayCursor;
    DWORD FlipWriteCursor;
};

struct win32_game_code
{
    HMODULE GameCodeDLL;
    FILETIME DLLLastWriteTime;

    // IMPORTANT(casey): Any of the callbacks can be 0!  You must
    // check before calling.
	game_initialize *Initialize;
	game_process_input *ProcessInput;
    game_update_and_render *UpdateAndRender;
    game_get_sound_samples *GetSoundSamples;

    bool32 IsValid;
};

#define WIN32_STATE_FILE_NAME_COUNT MAX_PATH
struct win32_replay_buffer
{
    HANDLE FileHandle;
    HANDLE MemoryMap;
    char FileName[WIN32_STATE_FILE_NAME_COUNT];
    void *MemoryBlock;
};
struct win32_state
{
    uint64 TotalSize = 0;
    void *GameMemoryBlock = nullptr;
	win32_replay_buffer ReplayBuffers[4] = {};
    
    HANDLE RecordingHandle = nullptr;
    int InputRecordingIndex = 0;

    HANDLE PlaybackHandle = nullptr;
    int InputPlayingIndex = 0;
    
	char EXEFileName[WIN32_STATE_FILE_NAME_COUNT] = {};
	uint32 EXEFileNameSize = 0;
    char *OnePastLastEXEFileNameSlash = nullptr;

	PlatformGameSettings Settings;
};

// TODO(casey): This is a global for now.
global_variable bool32 GlobalRunning;
global_variable bool32 GlobalPause;
//global_variable win32_offscreen_buffer GlobalBackbuffer;
//global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;
global_variable int64 GlobalPerfCountFrequency;
global_variable HDC GlobalDeviceContext;
global_variable PlatformGameSettings upcomingSettings;

const HMString defaultSettingsLocation = { "settings.txt" };

#endif
