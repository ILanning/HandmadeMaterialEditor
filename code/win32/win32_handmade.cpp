/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

/*
  TODO(casey):  THIS IS NOT A FINAL PLATFORM LAYER!!!

  - Saved game locations
  - Getting a handle to our own executable file
  - Asset loading path
  - Threading (launch a thread)
  - Raw Input (support for multiple keyboards)
  - Sleep/timeBeginPeriod
  - ClipCursor() (for multimonitor support)
  - Fullscreen support
  - WM_SETCURSOR (control cursor visibility)
  - QueryCancelAutoplay
  - WM_ACTIVATEAPP (for when we are not the active application)
  - GetKeyboardLayout (for French keyboards, international WASD support)
  - Tell game layer when a window resize has happened and handle it gracefully
  - Add a way to restrict the size/aspect ratio/resolution of the window to a set of certain values

  Just a partial list of stuff!!
*/

#include "..\handmade.h"

#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <xinput.h>
#include <dsound.h>
#ifndef GLEW_IMPORTED

#include "..\libraries\glew.h"
#include "..\libraries\wglew.h"
#define GLEW_IMPORTED 1

#endif

#include "win32_handmade.h"
#include "win32_utility.cpp"
#include "win32_audio.cpp"
#include "win32_input.cpp"
#include "win32_graphics.cpp"
#include "win32_debug.cpp"
#include "InputProcessor.cpp"
#include "../GameState.h"

internal win32_game_code Win32LoadGameCode(char *SourceDLLName, char *TempDLLName)
{
	win32_game_code Result = {};

    Result.DLLLastWriteTime = Win32GetLastWriteTime(SourceDLLName);

    CopyFile(SourceDLLName, TempDLLName, FALSE);
    
    Result.GameCodeDLL = LoadLibraryA(TempDLLName);
    if(Result.GameCodeDLL)
    {
		Result.Initialize = (game_initialize *)
			GetProcAddress(Result.GameCodeDLL, "GameInitialize");

		Result.HandleInput = (game_handle_input *)
			GetProcAddress(Result.GameCodeDLL, "GameHandleInput");

        Result.UpdateAndRender = (game_update_and_render *)
            GetProcAddress(Result.GameCodeDLL, "GameUpdateAndRender");
        
        Result.GetSoundSamples = (game_get_sound_samples *)
            GetProcAddress(Result.GameCodeDLL, "GameGetSoundSamples");

        Result.IsValid = (Result.Initialize && 
						  Result.HandleInput &&
						  Result.UpdateAndRender &&
                          Result.GetSoundSamples);
    }

    if(!Result.IsValid)
    {
		Result.Initialize = nullptr;
		Result.HandleInput = nullptr;
        Result.UpdateAndRender = nullptr;
        Result.GetSoundSamples = nullptr;
    }

    return(Result);
}

internal void Win32UnloadGameCode(win32_game_code *GameCode)
{
    if(GameCode->GameCodeDLL)
    {
        FreeLibrary(GameCode->GameCodeDLL);
        GameCode->GameCodeDLL = 0;
    }

    GameCode->IsValid = false;
	GameCode->Initialize = nullptr;
	GameCode->HandleInput = nullptr;
    GameCode->UpdateAndRender = nullptr;
    GameCode->GetSoundSamples = nullptr;
}

internal LRESULT CALLBACK Win32MainWindowCallback(HWND Window,
                        UINT Message,
                        WPARAM WParam,
                        LPARAM LParam)
{       
    LRESULT Result = 0;

    switch(Message)
    {
		case WM_DESTROY:
		{
			// TODO(casey): Handle this as an error - recreate window?
			GlobalRunning = false;
		} break;

        case WM_CLOSE:
        {
            // TODO(casey): Handle this with a message to the user?
            GlobalRunning = false;
        } break;

        case WM_ACTIVATEAPP:
        {
#if 0
            if(WParam == TRUE)
            {
                SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);
            }
            else
            {
                SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 64, LWA_ALPHA);
            }
#endif
        } break;

		case WM_MOVE:
		{
			upcomingSettings.WindowPosition.x = LOWORD(LParam);
			upcomingSettings.WindowPosition.y = HIWORD(LParam);
		} break;

		case WM_SIZE:
		{
			//TODO(Ian): Respect fullscreen/maximization
			upcomingSettings.WindowSize.x = LOWORD(LParam);
			upcomingSettings.WindowSize.y = HIWORD(LParam);
		} break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
		//case WM_INPUT:
        {
            Assert(!"Keyboard input came in through a non-dispatch message!");
        } break;

        default:
        {
//            OutputDebugStringA("default\n");
            Result = DefWindowProcA(Window, Message, WParam, LParam);
        } break;
    }
    
    return(Result);
}

internal void Win32ProcessPendingMessages(win32_state *State, game_controller_input *KeyboardController, InputProcessor *inputProcessor, PlatformGameSettings *changesFromWindows)
{
    MSG message;
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_QUIT:
            {
                GlobalRunning = false;
            } break;

			//TODO(Ian):  Handle WM_MOVE, WM_RESIZE, WM_DEVICECHANGE, WM_ACTIVATEAPP, etc.

			case WM_MOVE:
			{
				changesFromWindows->WindowPosition.x = LOWORD(message.lParam);
				changesFromWindows->WindowPosition.y = HIWORD(message.lParam);
			} break;

			case WM_INPUT:
			{
				inputProcessor->HandleRawInputMessages(message);
			} break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                /*
#if HANDMADE_INTERNAL
                    else if(VKCode == 'P')
                    {
                        if(IsDown)
                        {
                            GlobalPause = !GlobalPause;
                        }
                    }
                    else if(VKCode == 'L')
                    {
                        if(IsDown)
                        {
                            if(State->InputPlayingIndex == 0)
                            {
                                if(State->InputRecordingIndex == 0)
                                {
                                    Win32BeginRecordingInput(State, 1);
                                }
                                else
                                {
                                    Win32EndRecordingInput(State);
                                    Win32BeginInputPlayBack(State, 1);
                                }
                            }
                            else
                            {
                                Win32EndInputPlayBack(State);
                            }
                        }
                    }
#endif
                }

                bool32 AltKeyWasDown = (message.lParam & (1 << 29));
                if((VKCode == VK_F4) && AltKeyWasDown)
                {
                    GlobalRunning = false;
                }*/
            } break;

            default:
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            } break;
        }
    }
}

/**
 * \brief Creates an OpenGL context.
*/
HGLRC InitializeOpenGL()
{
	//NOTE(Ian): OpenGL Context Creation
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int32 pixelFormatNumber = ChoosePixelFormat(GlobalDeviceContext, &pfd);
	if (!pixelFormatNumber)
	{
		//TODO(Ian): Diagnostic
		exit(1);
	}
	SetPixelFormat(GlobalDeviceContext, pixelFormatNumber, &pfd);
	HGLRC dummyContext = wglCreateContext(GlobalDeviceContext);
	wglMakeCurrent(GlobalDeviceContext, dummyContext);

	if (GLEW_OK != glewInit())
	{
		//TODO(Ian): Diagnostic
		exit(1);
	}

	int glContextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	HGLRC glContext = wglCreateContextAttribsARB(GlobalDeviceContext, dummyContext, glContextAttribs);
	wglDeleteContext(dummyContext);

	return glContext;
}

void ReconcileSettingsChanges(PlatformGameSettings &futureSettings, PlatformGameSettings &win32Settings, PlatformGameSettings &gameSettings, HWND window)
{
	if (futureSettings.Fullscreen != gameSettings.Fullscreen)
	{
		if (gameSettings.Fullscreen)
		{
			HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
			MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
			GetMonitorInfo(monitor, &monitorInfo);
			int32 x = monitorInfo.rcMonitor.left;
			int32 y = monitorInfo.rcMonitor.top;
			int32 width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
			int32 height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

			futureSettings.cachedPosition = gameSettings.WindowPosition;
			futureSettings.cachedSize = gameSettings.WindowSize;
			futureSettings.WindowPosition = { (real32)x, (real32)y };
			futureSettings.WindowSize = { (real32)width, (real32)height };

			SetWindowLongPtr(window, GWL_STYLE,
				WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(window, x, y, width, height, TRUE);
		}
		else
		{
			RECT rect;
			rect.left = (int32)gameSettings.cachedPosition.x;
			rect.top = (int32)gameSettings.cachedPosition.y;
			rect.right = (int32)gameSettings.cachedSize.x + rect.left;
			rect.bottom = (int32)gameSettings.cachedSize.y + rect.top;

			futureSettings.WindowPosition = gameSettings.cachedPosition;
			futureSettings.WindowSize = gameSettings.cachedSize;

			SetWindowLongPtr(window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow(window, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
		futureSettings.Fullscreen = gameSettings.Fullscreen;
	}
	win32Settings = futureSettings;
}

int CALLBACK WinMain(HINSTANCE Instance,
					 HINSTANCE PrevInstance,
					 LPSTR CommandLine,
					 int ShowCode)
{
    win32_state Win32State = {};

    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    Win32GetEXEFileName(&Win32State);

    char SourceGameCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
    Win32BuildEXEPathFileName(&Win32State, "handmade.dll",
                              sizeof(SourceGameCodeDLLFullPath), SourceGameCodeDLLFullPath);

    char TempGameCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
    Win32BuildEXEPathFileName(&Win32State, "handmade_temp.dll",
                              sizeof(TempGameCodeDLLFullPath), TempGameCodeDLLFullPath);

    // NOTE(casey): Set the Windows scheduler granularity to 1ms
    // so that our Sleep() can be more granular.
    UINT DesiredSchedulerMS = 1;
    bool32 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);
    
    Win32LoadXInput();
    
    WNDCLASSA WindowClass = {};
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
//    WindowClass.hIcon;  //TODO(Ian): Add a window icon
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";

    if(RegisterClassA(&WindowClass))
    {
        HWND Window =
            CreateWindowExA(
                0, // WS_EX_TOPMOST|WS_EX_LAYERED,
                WindowClass.lpszClassName,
                "Handmade Material Editor",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                Instance,
                0);

        if(Window)
        {
            //win32_sound_output SoundOutput = {};

			GlobalDeviceContext = GetDC(Window);

            // TODO(casey): How do we reliably query on this on Windows?
            int MonitorRefreshHz = 60;
            int Win32RefreshRate = GetDeviceCaps(GlobalDeviceContext, VREFRESH);
            if(Win32RefreshRate > 1)
            {
                MonitorRefreshHz = Win32RefreshRate;
            }
            real32 GameUpdateHz = (MonitorRefreshHz / 2.0f);
            real32 TargetSecondsPerFrame = 1.0f / (real32)GameUpdateHz;

			GlobalRunning = true;

            // TODO(casey): Make this like sixty seconds?
            /*SoundOutput.SamplesPerSecond = 48000;
            SoundOutput.BytesPerSample = sizeof(int16)*2;
            SoundOutput.SecondaryBufferSize = SoundOutput.SamplesPerSecond*SoundOutput.BytesPerSample;
            // TODO(casey): Actually compute this variance and see
            // what the lowest reasonable value is.
            SoundOutput.SafetyBytes = (int)(((real32)SoundOutput.SamplesPerSecond*(real32)SoundOutput.BytesPerSample / GameUpdateHz)/3.0f);
            Win32InitDSound(Window, SoundOutput.SamplesPerSecond, SoundOutput.SecondaryBufferSize);
            Win32ClearBuffer(&SoundOutput);
            GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

#if 0
            // NOTE(casey): This tests the PlayCursor/WriteCursor update frequency
            // On the Handmade Hero machine, it was 480 samples.
            while(GlobalRunning)
            {
                DWORD PlayCursor;
                DWORD WriteCursor;
                GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor);

                char TextBuffer[256];
                _snprintf_s(TextBuffer, sizeof(TextBuffer),
                            "PC:%u WC:%u\n", PlayCursor, WriteCursor);
                OutputDebugStringA(TextBuffer);
            }
#endif
            
            // TODO(casey): Pool with bitmap VirtualAlloc
            int16 *Samples = (int16 *)VirtualAlloc(0, SoundOutput.SecondaryBufferSize,
                                                   MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); */

			InputProcessor inputProcessor = {};
			inputProcessor.RegisterRawInput();
            
#if HANDMADE_INTERNAL
            LPVOID BaseAddress = (LPVOID)Terabytes(2);
#else
            LPVOID BaseAddress = 0;
#endif
            
            game_memory GameMemory = {};
            GameMemory.PermanentStorageSize = Megabytes(64);
            GameMemory.TransientStorageSize = Megabytes(256);
			GameMemory.ReadEntireFile = DebugReadWrapper;
            GameMemory.DEBUGPlatformFreeFileMemory = DEBUGPlatformFreeFileMemory;
            GameMemory.DEBUGPlatformReadEntireFile = DEBUGPlatformReadEntireFile;
            GameMemory.DEBUGPlatformWriteEntireFile = DEBUGPlatformWriteEntireFile;
			GameMemory.DEBUGMessageError = VSOutputDebugString;


            // TODO(casey): Handle various memory footprints (USING SYSTEM METRICS)
            Win32State.TotalSize = GameMemory.PermanentStorageSize + GameMemory.TransientStorageSize;
            Win32State.GameMemoryBlock = VirtualAlloc(BaseAddress, (size_t)Win32State.TotalSize,
                                                      MEM_RESERVE|MEM_COMMIT,
                                                      PAGE_READWRITE);
            GameMemory.PermanentStorage = Win32State.GameMemoryBlock;
            GameMemory.TransientStorage = ((uint8 *)GameMemory.PermanentStorage +
                                           GameMemory.PermanentStorageSize);
			
			Win32State.Settings.SetWindowTitle("Handmade Material Editor", 25);
			Win32State.Settings.Focused = true;
			POINT clientPos = { 0, 0 };
			ClientToScreen(Window, &clientPos);
			Win32State.Settings.WindowPosition = { (real32)clientPos.x , (real32)clientPos.y };
			RECT clientBounds = {};
			GetClientRect(Window, &clientBounds);
			Win32State.Settings.WindowSize = { (real32)clientBounds.right, (real32)clientBounds.bottom };

			upcomingSettings = PlatformGameSettings(Win32State.Settings);

            for(int ReplayIndex = 0;
                ReplayIndex < ArrayCount(Win32State.ReplayBuffers);
                ++ReplayIndex)
            {
                win32_replay_buffer *ReplayBuffer = &Win32State.ReplayBuffers[ReplayIndex];

                // TODO(casey): Recording system still seems to take too long
                // on record start - find out what Windows is doing and if
                // we can speed up / defer some of that processing.
                
                Win32GetInputFileLocation(&Win32State, false, ReplayIndex,
                                          sizeof(ReplayBuffer->FileName), ReplayBuffer->FileName);

                ReplayBuffer->FileHandle =
                    CreateFileA(ReplayBuffer->FileName,
                                GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, 0, 0);

                LARGE_INTEGER MaxSize;
                MaxSize.QuadPart = Win32State.TotalSize;
                ReplayBuffer->MemoryMap = CreateFileMapping(
                    ReplayBuffer->FileHandle, 0, PAGE_READWRITE,
                    MaxSize.HighPart, MaxSize.LowPart, 0);

                ReplayBuffer->MemoryBlock = MapViewOfFile(
                    ReplayBuffer->MemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, Win32State.TotalSize);
                if(ReplayBuffer->MemoryBlock)
                {
                }
                else
                {
                    // TODO(casey): Diagnostic
                }
            }

			HGLRC glContext = InitializeOpenGL();
			wglMakeCurrent(GlobalDeviceContext, glContext);
			
            if(GameMemory.PermanentStorage && GameMemory.TransientStorage && glContext)
            {
                GameInput Input[2] = {};
				GameInput *NewInput = &Input[0];
				GameInput *OldInput = &Input[1];
    
                LARGE_INTEGER LastCounter = Win32GetWallClock();
                LARGE_INTEGER FlipWallClock = Win32GetWallClock();

                int DebugTimeMarkerIndex = 0;
                win32_debug_time_marker DebugTimeMarkers[30] = {0};

                DWORD AudioLatencyBytes = 0;
                real32 AudioLatencySeconds = 0;
                bool32 SoundIsValid = false;

                win32_game_code Game = Win32LoadGameCode(SourceGameCodeDLLFullPath,
                                                         TempGameCodeDLLFullPath);
                uint32 LoadCounter = 0;

				if (Game.Initialize)
				{
					thread_context dummyThreadContext = { 1 };
					Game.Initialize(&dummyThreadContext, &GameMemory);
				}

                uint64 LastCycleCount = __rdtsc();

				//BEGIN MAIN GAME LOOP

				while (GlobalRunning)
				{
					FILETIME NewDLLWriteTime = Win32GetLastWriteTime(SourceGameCodeDLLFullPath);
					if (CompareFileTime(&NewDLLWriteTime, &Game.DLLLastWriteTime) != 0)
					{
						Win32UnloadGameCode(&Game);
						Game = Win32LoadGameCode(SourceGameCodeDLLFullPath,
							TempGameCodeDLLFullPath);
						LoadCounter = 0;
					}

					//START KEYBOARD

					game_controller_input *OldKeyboardController = GetController(OldInput, 0);
					game_controller_input *NewKeyboardController = GetController(NewInput, 0);
					*NewKeyboardController = {};
					NewKeyboardController->IsConnected = true;
					for (int ButtonIndex = 0;
						ButtonIndex < ArrayCount(NewKeyboardController->Buttons);
						++ButtonIndex)
					{
						NewKeyboardController->Buttons[ButtonIndex].EndedDown =
							OldKeyboardController->Buttons[ButtonIndex].EndedDown;
					}

					Win32ProcessPendingMessages(&Win32State, NewKeyboardController, &inputProcessor, &upcomingSettings);

					//END KEYBOARD
					//START MOUSE

					POINT MouseP;
					GetCursorPos(&MouseP);
					ScreenToClient(Window, &MouseP);

					inputProcessor.nextFrame.MousePos = { (real32)MouseP.x, (real32)MouseP.y };

					NewInput->newFrame = inputProcessor.GenerateFrame();

					//END MOUSE
					//START CONTROLLER

					// TODO(casey): Need to not poll disconnected controllers to avoid
					// xinput frame rate hit on older libraries...
					// TODO(casey): Should we poll this more frequently
					DWORD MaxControllerCount = XUSER_MAX_COUNT;
					if (MaxControllerCount > (ArrayCount(NewInput->Controllers) - 1))
					{
						MaxControllerCount = (ArrayCount(NewInput->Controllers) - 1);
					}

					for (DWORD ControllerIndex = 0;
						ControllerIndex < MaxControllerCount;
						++ControllerIndex)
					{
						DWORD OurControllerIndex = ControllerIndex + 1;
						game_controller_input *OldController = GetController(OldInput, OurControllerIndex);
						game_controller_input *NewController = GetController(NewInput, OurControllerIndex);

						XINPUT_STATE ControllerState;
						if (XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
						{
							NewController->IsConnected = true;
							NewController->IsAnalog = OldController->IsAnalog;

							// NOTE(casey): This controller is plugged in
							// TODO(casey): See if ControllerState.dwPacketNumber increments too rapidly
							XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

							// TODO(casey): This is a square deadzone, check XInput to
							// verify that the deadzone is "round" and show how to do
							// round deadzone processing.
							NewController->StickAverageX = Win32ProcessXInputStickValue(
								Pad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
							NewController->StickAverageY = Win32ProcessXInputStickValue(
								Pad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
							if ((NewController->StickAverageX != 0.0f) ||
								(NewController->StickAverageY != 0.0f))
							{
								NewController->IsAnalog = true;
							}

							if (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP)
							{
								NewController->StickAverageY = 1.0f;
								NewController->IsAnalog = false;
							}

							if (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
							{
								NewController->StickAverageY = -1.0f;
								NewController->IsAnalog = false;
							}

							if (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
							{
								NewController->StickAverageX = -1.0f;
								NewController->IsAnalog = false;
							}

							if (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
							{
								NewController->StickAverageX = 1.0f;
								NewController->IsAnalog = false;
							}

							real32 Threshold = 0.5f;
							Win32ProcessXInputDigitalButton(
								(NewController->StickAverageX < -Threshold) ? 1 : 0,
								&OldController->MoveLeft, 1,
								&NewController->MoveLeft);
							Win32ProcessXInputDigitalButton(
								(NewController->StickAverageX > Threshold) ? 1 : 0,
								&OldController->MoveRight, 1,
								&NewController->MoveRight);
							Win32ProcessXInputDigitalButton(
								(NewController->StickAverageY < -Threshold) ? 1 : 0,
								&OldController->MoveDown, 1,
								&NewController->MoveDown);
							Win32ProcessXInputDigitalButton(
								(NewController->StickAverageY > Threshold) ? 1 : 0,
								&OldController->MoveUp, 1,
								&NewController->MoveUp);

							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->ActionDown, XINPUT_GAMEPAD_A,
								&NewController->ActionDown);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->ActionRight, XINPUT_GAMEPAD_B,
								&NewController->ActionRight);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->ActionLeft, XINPUT_GAMEPAD_X,
								&NewController->ActionLeft);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->ActionUp, XINPUT_GAMEPAD_Y,
								&NewController->ActionUp);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->LeftShoulder, XINPUT_GAMEPAD_LEFT_SHOULDER,
								&NewController->LeftShoulder);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->RightShoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER,
								&NewController->RightShoulder);

							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->Start, XINPUT_GAMEPAD_START,
								&NewController->Start);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
								&OldController->Back, XINPUT_GAMEPAD_BACK,
								&NewController->Back);
						}
						else
						{
							// NOTE(casey): The controller is not available
							NewController->IsConnected = false;
						}
					}

					//END CONTROLLER

					thread_context Thread = {};

					if (Win32State.InputRecordingIndex)
					{
						Win32RecordInput(&Win32State, NewInput);
					}

					if (Win32State.InputPlayingIndex)
					{
						Win32PlayBackInput(&Win32State, NewInput);
					}
					if (Game.HandleInput)
					{
						Game.HandleInput(&Thread, NewInput, &GameMemory, &upcomingSettings);
					}
					if (Game.UpdateAndRender)
					{
						Game.UpdateAndRender(&Thread, &GameMemory);
					}
					SwapBuffers(GlobalDeviceContext);

					/* LARGE_INTEGER AudioWallClock = Win32GetWallClock();
					 real32 FromBeginToAudioSeconds = Win32GetSecondsElapsed(FlipWallClock, AudioWallClock);

					 DWORD PlayCursor;
					 DWORD WriteCursor;
					 if(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor) == DS_OK)
					 {
						 /* NOTE(casey):

							Here is how sound output computation works.

							We define a safety value that is the number
							of samples we think our game update loop
							may vary by (let's say up to 2ms)

							When we wake up to write audio, we will look
							and see what the play cursor position is and we
							will forecast ahead where we think the play
							cursor will be on the next frame boundary.

							We will then look to see if the write cursor is
							before that by at least our safety value.  If
							it is, the target fill position is that frame
							boundary plus one frame.  This gives us perfect
							audio sync in the case of a card that has low
							enough latency.

							If the write cursor is _after_ that safety
							margin, then we assume we can never sync the
							audio perfectly, so we will write one frame's
							worth of audio plus the safety margin's worth
							of guard samples.
						 *
						 if(!SoundIsValid)
						 {
							 SoundOutput.RunningSampleIndex = WriteCursor / SoundOutput.BytesPerSample;
							 SoundIsValid = true;
						 }

						 DWORD ByteToLock = ((SoundOutput.RunningSampleIndex*SoundOutput.BytesPerSample) %
											 SoundOutput.SecondaryBufferSize);

						 DWORD ExpectedSoundBytesPerFrame =
							 (int)((real32)(SoundOutput.SamplesPerSecond*SoundOutput.BytesPerSample) /
								   GameUpdateHz);
						 real32 SecondsLeftUntilFlip = (TargetSecondsPerFrame - FromBeginToAudioSeconds);
						 DWORD ExpectedBytesUntilFlip = (DWORD)((SecondsLeftUntilFlip/TargetSecondsPerFrame)*(real32)ExpectedSoundBytesPerFrame);

						 DWORD ExpectedFrameBoundaryByte = PlayCursor + ExpectedBytesUntilFlip;

						 DWORD SafeWriteCursor = WriteCursor;
						 if(SafeWriteCursor < PlayCursor)
						 {
							 SafeWriteCursor += SoundOutput.SecondaryBufferSize;
						 }
						 Assert(SafeWriteCursor >= PlayCursor);
						 SafeWriteCursor += SoundOutput.SafetyBytes;

						 bool32 AudioCardIsLowLatency = (SafeWriteCursor < ExpectedFrameBoundaryByte);

						 DWORD TargetCursor = 0;
						 if(AudioCardIsLowLatency)
						 {
							 TargetCursor = (ExpectedFrameBoundaryByte + ExpectedSoundBytesPerFrame);
						 }
						 else
						 {
							 TargetCursor = (WriteCursor + ExpectedSoundBytesPerFrame +
											 SoundOutput.SafetyBytes);
						 }
						 TargetCursor = (TargetCursor % SoundOutput.SecondaryBufferSize);

						 DWORD BytesToWrite = 0;
						 if(ByteToLock > TargetCursor)
						 {
							 BytesToWrite = (SoundOutput.SecondaryBufferSize - ByteToLock);
							 BytesToWrite += TargetCursor;
						 }
						 else
						 {
							 BytesToWrite = TargetCursor - ByteToLock;
						 }

						 game_sound_output_buffer SoundBuffer = {};
						 SoundBuffer.SamplesPerSecond = SoundOutput.SamplesPerSecond;
						 SoundBuffer.SampleCount = BytesToWrite / SoundOutput.BytesPerSample;
						 SoundBuffer.Samples = Samples;
						 if(Game.GetSoundSamples)
						 {
							 Game.GetSoundSamples(&Thread, &GameMemory, &SoundBuffer);
						 }

#if HANDMADE_INTERNAL
							win32_debug_time_marker *Marker = &DebugTimeMarkers[DebugTimeMarkerIndex];
							Marker->OutputPlayCursor = PlayCursor;
							Marker->OutputWriteCursor = WriteCursor;
							Marker->OutputLocation = ByteToLock;
							Marker->OutputByteCount = BytesToWrite;
							Marker->ExpectedFlipPlayCursor = ExpectedFrameBoundaryByte;

							DWORD UnwrappedWriteCursor = WriteCursor;
							if(UnwrappedWriteCursor < PlayCursor)
							{
								UnwrappedWriteCursor += SoundOutput.SecondaryBufferSize;
							}
							AudioLatencyBytes = UnwrappedWriteCursor - PlayCursor;
							AudioLatencySeconds =
								(((real32)AudioLatencyBytes / (real32)SoundOutput.BytesPerSample) /
								 (real32)SoundOutput.SamplesPerSecond);

#if 0
							char TextBuffer[256];
							_snprintf_s(TextBuffer, sizeof(TextBuffer),
										"BTL:%u TC:%u BTW:%u - PC:%u WC:%u DELTA:%u (%fs)\n",
										ByteToLock, TargetCursor, BytesToWrite,
										PlayCursor, WriteCursor, AudioLatencyBytes, AudioLatencySeconds);
							OutputDebugStringA(TextBuffer);
#endif
#endif
							Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
						}
						else
						{
							SoundIsValid = false;
						}*/

					PlatformGameSettings &frameFinalSettings = ((GameState *)GameMemory.PermanentStorage)->WindowSettings;
					if (Win32State.Settings != frameFinalSettings)
					{
						ReconcileSettingsChanges(upcomingSettings, Win32State.Settings, frameFinalSettings, Window);
					}

					LARGE_INTEGER WorkCounter = Win32GetWallClock();
					real32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);

					// TODO(casey): NOT TESTED YET!  PROBABLY BUGGY!!!!!
					real32 SecondsElapsedForFrame = WorkSecondsElapsed;
					if (SecondsElapsedForFrame < TargetSecondsPerFrame)
					{
						if (SleepIsGranular)
						{
							DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame -
								SecondsElapsedForFrame));
							if (SleepMS > 0)
							{
								Sleep(SleepMS);
							}
						}

						real32 TestSecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter,
							Win32GetWallClock());
						if (TestSecondsElapsedForFrame < TargetSecondsPerFrame)
						{
							// TODO(casey): LOG MISSED SLEEP HERE
						}

						while (SecondsElapsedForFrame < TargetSecondsPerFrame)
						{
							SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter,
								Win32GetWallClock());
						}
					}
					else
					{
						// TODO(casey): MISSED FRAME RATE!
						// TODO(casey): Logging
					}

					LARGE_INTEGER EndCounter = Win32GetWallClock();
					real32 MSPerFrame = 1000.0f*Win32GetSecondsElapsed(LastCounter, EndCounter);
					LastCounter = EndCounter;

					//win32_window_dimension Dimension = Win32GetWindowDimension(Window);
					//HDC DeviceContext = GetDC(Window);
					//Win32DisplayBufferInWindow(&GlobalBackbuffer, GlobalDeviceContext,
					//                           Dimension.Width, Dimension.Height);
					//ReleaseDC(Window, DeviceContext);

					FlipWallClock = Win32GetWallClock();
#if HANDMADE_INTERNAL
					// NOTE(casey): This is debug code
					/*{
						PlayCursor = 0;
						WriteCursor = 0;
						if(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor) == DS_OK)
						{
							Assert(DebugTimeMarkerIndex < ArrayCount(DebugTimeMarkers));
							win32_debug_time_marker *Marker = &DebugTimeMarkers[DebugTimeMarkerIndex];
							Marker->FlipPlayCursor = PlayCursor;
							Marker->FlipWriteCursor = WriteCursor;
						}

					}*/
#endif

					GameInput *Temp = NewInput;
					NewInput = OldInput;
					OldInput = Temp;
					// TODO(casey): Should I clear these here?

#if 0
					uint64 EndCycleCount = __rdtsc();
					uint64 CyclesElapsed = EndCycleCount - LastCycleCount;
					LastCycleCount = EndCycleCount;

					real64 FPS = 0.0f;
					real64 MCPF = ((real64)CyclesElapsed / (1000.0f * 1000.0f));

					char FPSBuffer[256];
					_snprintf_s(FPSBuffer, sizeof(FPSBuffer),
						"%.02fms/f,  %.02ff/s,  %.02fmc/f\n", MSPerFrame, FPS, MCPF);
					OutputDebugStringA(FPSBuffer);
#endif

#if HANDMADE_INTERNAL
					++DebugTimeMarkerIndex;
					if (DebugTimeMarkerIndex == ArrayCount(DebugTimeMarkers))
					{
						DebugTimeMarkerIndex = 0;
					}
#endif
					//}
				}
            
				//END MAIN GAME LOOP
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
    }
    else
    {
        // TODO(casey): Logging
    }
    
    return(0);
}
