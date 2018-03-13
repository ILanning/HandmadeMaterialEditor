#ifndef HANDMADE_WIN32_INPUTPROCESSOR_H
#define HANDMADE_WIN32_INPUTPROCESSOR_H

#include "../input/InputFrame.h"

/*
Needs to create InputFrames for the InputManager
Eventually will also handle keeping track of which devices are and aren't plugged in
*/
struct InputProcessor
{
	Input::InputFrame nextFrame;

	void RegisterRawInput();

	void HandleRawInputMessages(MSG message);

	//TODO(Ian): Have this accept timing information in support of frametime-invariant updating
	Input::InputFrame GenerateFrame()
	{
		Input::InputFrame clone = nextFrame;
		nextFrame.scrollWheelChange = 0;
		return clone;
	}
};

#endif