#ifndef HANDMADE_TESTSCENE_H
#define HANDMADE_TESTSCENE_H

#include "handmade_typedefs.h"
#include "handmade_funcdefs.h"
#include "drawing\Model.h"
#include "drawing\Sprite.h"
#include "drawing\cameras\Camera2D.h"

struct GameState;

struct TestScene
{
	Drawing::Model *arrow;
	Drawing::Sprite *enterButton;
	Drawing::Model *Virt;
	Drawing::Camera2D *Camera;

	GLuint BuildShaderProgram(char *vertexSourcePath, char *fragmentSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError);

	void Initialize(ReadFileFunc *readFile, DebugMessageErrorFunc *messageError, GameState *gameState);

	void HandleInput(GameState *state);

	void Update(GameState *state);

	void Draw(GameState *state);
};

#endif //HANDMADE_TESTSCENE_H