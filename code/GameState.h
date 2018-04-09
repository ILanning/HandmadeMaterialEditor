#ifndef HANDMADE_GAMESTATE_H
#define HANDMADE_GAMESTATE_H

#include "drawing\Model.h"
#include "drawing\Sprite.h"
#include "drawing\cameras\Camera2D.h"
#include "input\InputManager.h"
#include "PlatformGameSettings.h"
#include "RescalerManager.h"


struct TestGlobals
{
	Drawing::Model *arrow;
	Drawing::Sprite *enterButton;
	Drawing::Model *Virt;

	Drawing::Camera2D *Camera;
};

struct GameState
{
	PlatformGameSettings WindowSettings;
	Input::InputManager Input;
	RescalerManager Rescalers;
	TestGlobals globals;
};

#endif //HANDMADE_GAMESTATE_H