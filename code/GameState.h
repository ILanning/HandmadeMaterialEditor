#ifndef HANDMADE_GAMESTATE
#define HANDMADE_GAMESTATE

#include "drawing\Model.h"
#include "drawing\ThirdPersonCamera.h"


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
};

#endif