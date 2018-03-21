#ifndef HANDMADE_GAMESTATE_H
#define HANDMADE_GAMESTATE_H

#include "drawing\Model.h"
#include "drawing\cameras\SphericalCamera.h"
#include "input\InputManager.h"
#include "PlatformGameSettings.h"
#include "RescalerManager.h"


struct TestGlobals
{
	Drawing::Model *arrow;
	Drawing::Model *enterButton;
	Drawing::Model *enter2;
	Drawing::Model *Virt;

	Drawing::SphericalCamera *Camera;
};

struct GameState
{
	PlatformGameSettings WindowSettings;
	Input::InputManager Input;
	RescalerManager Rescalers;
	TestGlobals globals;
};

#endif //HANDMADE_GAMESTATE_H