#ifndef HANDMADE_GAMESTATE_H
#define HANDMADE_GAMESTATE_H

#include "input\InputManager.h"
#include "PlatformGameSettings.h"
#include "RescalerManager.h"
#include "TestScene.h"

struct GameState
{
	PlatformGameSettings WindowSettings;
	Input::InputManager Input;
	RescalerManager Rescalers;
	TestScene testScene;
};

#endif //HANDMADE_GAMESTATE_H