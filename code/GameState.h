#ifndef HANDMADE_GAMESTATE
#define HANDMADE_GAMESTATE

#include "drawing\Model.h"
#include "drawing\cameras\SphericalCamera.h"
#include "input\InputManager.h"
#include "PlatformGameSettings.h"


struct TestGlobals
{
	Model *arrow;
	Model *enterButton;
	Model *enter2;
	Model *Virt;

	Drawing::SphericalCamera *Camera;
};

struct GameState
{
	TestGlobals globals;
	Input::InputManager Input;
	PlatformGameSettings WindowSettings;
};

#endif