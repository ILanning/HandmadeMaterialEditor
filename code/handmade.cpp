#ifndef HANDMADE_CPP
#define HANDMADE_CPP
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

#include "handmade.h"
#include "math\Vector2.h"
#include "math\Vector3.h"
#include "math\Quaternion.h"
#include "math\Matrix3.h"
#include "math\Matrix4.h"
#include "general\Assert.h"
#include "PlatformGameSettings.cpp"
#include "GameState.h"
#include "TestScene.cpp"

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

extern "C" GAME_INITIALIZE(GameInitialize)
{
	GameState *state = (GameState*)memory->PermanentStorage;

	state->testScene.Initialize(memory->ReadEntireFile, memory->DEBUGMessageError, state);
}

extern "C" GAME_PROCESS_INPUT(GameProcessInput)
{
	Assert((&newInputs->Controllers[0].Terminator - &newInputs->Controllers[0].Buttons[0]) ==
		(ArrayCount(newInputs->Controllers[0].Buttons)));

	if (!memory->IsInitialized)
	{
		InitMemory(thread, memory);
	}

	GameState &gameState = *(GameState *)memory->PermanentStorage;

	if (gameState.WindowSettings != *updatedSettings)
	{
		if (gameState.WindowSettings.WindowSize != updatedSettings->WindowSize)
		{
			gameState.Rescalers.RescaleGraphics(gameState.WindowSettings.WindowSize, updatedSettings->WindowSize, 1);
			glViewport(0, 0, (GLsizei)updatedSettings->WindowSize.x, (GLsizei)updatedSettings->WindowSize.y);
		}
		gameState.WindowSettings = PlatformGameSettings(*updatedSettings);
	}

	Input::InputManager &inputManager = gameState.Input;
	inputManager.HandleInput(newInputs);
	
	if(inputManager.ComboTriggered(Input::Alt, Input::Enter))
	{
		gameState.WindowSettings.Fullscreen = !gameState.WindowSettings.Fullscreen;
	}

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

	gameState->testScene.HandleInput(gameState);
	gameState->testScene.Update(gameState);
	gameState->testScene.Draw(gameState);
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
	GameState *gameState = (GameState *)Memory->PermanentStorage;
    //GameOutputSound(gameState, SoundBuffer, gameState->ToneHz);
}

#endif //HANDMADE_CPP