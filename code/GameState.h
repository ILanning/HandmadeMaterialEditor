#ifndef HANDMADE_GAMESTATE
#define HANDMADE_GAMESTATE

#include "drawing\Model.h"
#include "drawing\ThirdPersonCamera.h"
#include "input\InputManager.h"


struct TestGlobals
{
	Model *arrow;
	Model *enterButton;
	Model *enter2;
	Model *Virt;

	Drawing::ThirdPersonCamera Camera;
};

struct GameState
{
	TestGlobals globals;
	Input::InputManager Input;
};

#endif