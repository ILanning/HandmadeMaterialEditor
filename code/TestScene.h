#ifndef HANDMADE_TESTSCENE_H
#define HANDMADE_TESTSCENE_H

#include "handmade_typedefs.h"
#include "handmade_funcdefs.h"
#include "content\AssetManager.h"
#include "drawing\Model.h"
#include "drawing\Sprite.h"
#include "drawing\cameras\Camera2D.h"
#include "general\HMString.h"
#include "general\memory\NewDeleteArena.h"

struct GameState;

struct TestScene
{
	AssetManager content;
	Memory::NewDeleteArena memory;
	Drawing::Model *arrow;
	Drawing::Sprite *enterButton;
	Drawing::Model *Virt;
	Drawing::Camera2D *Camera;

	void Initialize(ReadFileFunc *readFile, DebugMessageErrorFunc *messageError, GameState *gameState);

	void HandleInput(GameState *state);

	void Update(GameState *state);

	void Draw(GameState *state);
};

#endif //HANDMADE_TESTSCENE_H