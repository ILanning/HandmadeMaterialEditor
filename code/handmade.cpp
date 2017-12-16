/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

#include "handmade.h"
#include "GameState.h"
#include "HandmadeRender.cpp"
#include "drawing\Model.h"
#include "drawing\ThirdPersonCamera.h"
#include "math\Vector2.h"
#include "math\Vector3.h"
#include "math\Quaternion.h"
#include "math\Matrix3.h"
#include "math\Matrix4.h"

/*internal void GameOutputSound(game_state *GameState, game_sound_output_buffer *SoundBuffer, int ToneHz)
{
    int16 ToneVolume = 3000;
    int WavePeriod = SoundBuffer->SamplesPerSecond/ToneHz;

    int16 *SampleOut = SoundBuffer->Samples;
    for(int SampleIndex = 0;
        SampleIndex < SoundBuffer->SampleCount;
        ++SampleIndex)
    {
        // TODO(casey): Draw this out for people
#if 1
        real32 SineValue = sinf(GameState->tSine);
        int16 SampleValue = (int16)(SineValue * ToneVolume);
#else
        int16 SampleValue = 0;
#endif
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;

        GameState->tSine += 2.0f*Pi32*1.0f/(real32)WavePeriod;
        if(GameState->tSine > 2.0f*Pi32)
        {
            GameState->tSine -= 2.0f*Pi32;
        }
    }
}

internal void
RenderWeirdGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    // TODO(casey): Let's see what the optimizer does

    uint8 *Row = (uint8 *)Buffer->Memory;    
    for(int Y = 0;
        Y < Buffer->Height;
        ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = 0;
            X < Buffer->Width;
            ++X)
        {
            uint8 Blue = (uint8)(X + BlueOffset);
            uint8 Green = (uint8)(Y + GreenOffset);

            *Pixel++ = ((Green << 16) | Blue);
        }
        
        Row += Buffer->Pitch;
    }
}

internal void
RenderPlayer(game_offscreen_buffer *Buffer, int PlayerX, int PlayerY)
{
    uint8 *EndOfBuffer = (uint8 *)Buffer->Memory + Buffer->Pitch*Buffer->Height;
    
    uint32 Color = 0xFFFFFFFF;
    int Top = PlayerY;
    int Bottom = PlayerY+10;
    for(int X = PlayerX;
        X < PlayerX+10;
        ++X)
    {
        uint8 *Pixel = ((uint8 *)Buffer->Memory +
                        X*Buffer->BytesPerPixel +
                        Top*Buffer->Pitch);
        for(int Y = Top;
            Y < Bottom;
            ++Y)
        {
            if((Pixel >= Buffer->Memory) &&
               ((Pixel + 4) <= EndOfBuffer))
            {
                *(uint32 *)Pixel = Color;
            }

            Pixel += Buffer->Pitch;
        }
    }
}*/

DebugMessageErrorFunc *MessageError;

bool InitMemory(thread_context *thread, game_memory *memory)
{
	char *filename = __FILE__;

	bool success = false;
	debug_read_file_result file = memory->DEBUGPlatformReadEntireFile(thread, filename, &success);
	if (success)
	{
		memory->DEBUGPlatformWriteEntireFile(thread, "test.out", file.ContentsSize, file.Contents);
		memory->DEBUGPlatformFreeFileMemory(thread, file.Contents);
	}

	// TODO(casey): This may be more appropriate to do in the platform layer
	// TODO(Ian): Add actual error checking here
	memory->IsInitialized = true;

	return success && memory->IsInitialized;
}

extern "C" GAME_HANDLE_INPUT(GameHandleInput)
{
	Assert((&newInputs->Controllers[0].Terminator - &newInputs->Controllers[0].Buttons[0]) ==
		(ArrayCount(newInputs->Controllers[0].Buttons)));

	if (!memory->IsInitialized)
	{
		InitMemory(thread, memory);
	}

	GameState *gameState = (GameState *)memory->PermanentStorage;

	Input::InputManager &inputManager = gameState->Input;
	inputManager.HandleInput(newInputs);

	for (int ControllerIndex = 0;
		ControllerIndex < ArrayCount(newInputs->Controllers);
		++ControllerIndex)
	{
		game_controller_input *Controller = GetController(newInputs, ControllerIndex);
		if (Controller->IsAnalog)
		{
			// NOTE(casey): Use analog movement tuning
			//GameState->BlueOffset += (int)(4.0f*Controller->StickAverageX);
			//GameState->ToneHz = 512 + (int)(128.0f*Controller->StickAverageY);
		}
		else
		{
			// NOTE(casey): Use digital movement tuning
			if (Controller->MoveLeft.EndedDown)
			{
				//GameState->BlueOffset -= 1;
			}

			if (Controller->MoveRight.EndedDown)
			{
				//GameState->BlueOffset += 1;
			}
		}
	}

	gameState->globals.Camera.HandleInput(&inputManager);
}

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    Assert(sizeof(GameState) <= Memory->PermanentStorageSize);
    
	GameState *gameState = (GameState *)Memory->PermanentStorage;
    if(!Memory->IsInitialized)
    {
		InitMemory(Thread, Memory);
    }

	if (!MessageError)
		MessageError = Memory->DEBUGMessageError;    
	    
	RenderScene(gameState);
    /*RenderWeirdGradient(Buffer, GameState->BlueOffset, GameState->GreenOffset);
    RenderPlayer(Buffer, GameState->PlayerX, GameState->PlayerY);

    RenderPlayer(Buffer, Input->MouseX, Input->MouseY);

    for(int ButtonIndex = 0;
        ButtonIndex < ArrayCount(Input->MouseButtons);
        ++ButtonIndex)
    {
        if(Input->MouseButtons[ButtonIndex].EndedDown)
        {
            RenderPlayer(Buffer, 10 + 20*ButtonIndex, 10);
        }
    }*/
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
	GameState *gameState = (GameState *)Memory->PermanentStorage;
    //GameOutputSound(gameState, SoundBuffer, gameState->ToneHz);
}
