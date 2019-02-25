#ifndef HANDMADE_H
#define HANDMADE_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

/*
  NOTE(casey):

  HANDMADE_INTERNAL:
    0 - Build for public release
    1 - Build for developer only

  HANDMADE_SLOW:
    0 - No slow code allowed!
    1 - Slow code welcome.
*/

#include "handmade_typedefs.h"
#include "handmade_funcdefs.h"
#include "handmade_debugfuncs.h"
#include "ThreadContext.h"
#include "input\InputFrame.h"
#include "general\Assert.h"
#include "PlatformGameSettings.h"

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
// TODO(casey): swap, min, max ... macros???

//TODO: Split the things in this file off as they become more certain

/*
  NOTE(casey): Services that the game provides to the platform layer.
  (this may expand in the future - sound on separate thread, etc.)
*/

// THREE THINGS - timing, controller/keyboard input, sound buffer to use

//NOTE: Game now directly tied to OpenGL, pixel buffer no longer needed

struct game_sound_output_buffer
{
    int SamplesPerSecond;
    int SampleCount;
    int16 *Samples;
};

struct game_button_state
{
    int HalfTransitionCount;
    bool32 EndedDown;
};

struct game_controller_input
{
    bool32 IsConnected;
    bool32 IsAnalog;    
    real32 StickAverageX;
    real32 StickAverageY;
    
    union
    {
        game_button_state Buttons[12];
        struct
        {
            game_button_state MoveUp;
            game_button_state MoveDown;
            game_button_state MoveLeft;
            game_button_state MoveRight;
            
            game_button_state ActionUp;
            game_button_state ActionDown;
            game_button_state ActionLeft;
            game_button_state ActionRight;
            
            game_button_state LeftShoulder;
            game_button_state RightShoulder;

            game_button_state Back;
            game_button_state Start;

            // NOTE(casey): All buttons must be added above this line
            
            game_button_state Terminator;
        };
    };
};

//TODO: Rework this, probably needs to have info on what devices we got input from (array of InputFrames?)
//      Plus info on device changes
struct GameInput
{
	Input::InputFrame newFrame;
    // TODO: Insert clock values here, delta from last frame and total time since program start.    
    game_controller_input Controllers[5];
};

inline game_controller_input *GetController(GameInput *Input, int unsigned ControllerIndex)
{
    Assert(ControllerIndex < ArrayCount(Input->Controllers));
    
    game_controller_input *Result = &Input->Controllers[ControllerIndex];
    return(Result);
}

struct game_memory
{
    bool32 IsInitialized;

    uint64 PermanentStorageSize;
    void *PermanentStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

    uint64 TransientStorageSize;
    void *TransientStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

	ReadFileFunc *ReadEntireFile;
	WriteFileFunc *WriteEntireFile;

    debug_platform_free_file_memory *DEBUGPlatformFreeFileMemory;
    debug_platform_read_entire_file *DEBUGPlatformReadEntireFile;
    debug_platform_write_entire_file *DEBUGPlatformWriteEntireFile;
	DebugMessageErrorFunc *DEBUGMessageError;
};

#define GAME_INITIALIZE(name) void name(ThreadContext *thread, game_memory *memory)
typedef GAME_INITIALIZE(game_initialize);

#define GAME_PROCESS_INPUT(name) void name(ThreadContext *thread, GameInput *newInputs, game_memory *memory, PlatformGameSettings *updatedSettings)
typedef GAME_PROCESS_INPUT(game_process_input);

#define GAME_UPDATE_AND_RENDER(name) void name(ThreadContext *Thread, game_memory *Memory)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

// NOTE(casey): At the moment, this has to be a very fast function, it cannot be
// more than a millisecond or so.
// TODO(casey): Reduce the pressure on this function's performance by measuring it
// or asking about it, etc.
#define GAME_GET_SOUND_SAMPLES(name) void name(ThreadContext *Thread, game_memory *Memory, game_sound_output_buffer *SoundBuffer)
typedef GAME_GET_SOUND_SAMPLES(game_get_sound_samples);

#endif //HANDMADE_H
