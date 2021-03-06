#ifndef HANDMADE_TESTSCENE_CPP
#define HANDMADE_TESTSCENE_CPP

#include "TestScene.h"

#include "GameState.h"
#include "content\AssetManager.h"
#include "content\MeshCollection.h"
#include "content\OBJLoader.h"
#include "drawing\GeometryHelpers.h"
#include "drawing\GLState.h"
#include "drawing\Sprite.h"
#include "drawing\Texture2D.h"
#include "drawing\Vertex.h"
#include "general\StringHelpers.h"
#include "math\Matrix4.h"
#include "math\Vector2.h"
#include "math\Vector3.h"



void TestScene::Initialize(ReadFileFunc *readFile, DebugMessageErrorFunc *messageError, GameState *gameState)
{
	//DebugOutputGLErrors(messageError);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //Uncomment to get a wireframe view

	memory = Memory::NewDeleteArena();
	content = AssetManager(readFile, memory);
	GLuint shaderProgram = content.CreateShader({ "default" }, { "Shaders\\Basic.vert" }, { "Shaders\\Basic.frag" }, messageError);
	content.Shaders.DefaultShader = shaderProgram;
	glUseProgram(shaderProgram);

	bool success = false;
	Drawing::Material tempEnterMat = Drawing::Material(content.Load<Drawing::Texture2D>("EnterButton.png", success));
	AssetPtr<Drawing::Material> enterMat = content.AddManaged(tempEnterMat, "EnterMat", success);
	enterButton = new Drawing::Sprite(enterMat, shaderProgram);
	//enterButton->SetSampleArea({ 75, 0, 40, 66 });

	Virt = new Drawing::Model(content.Load<Content::MeshCollection>({ "Assets/chair/chair.obj" }, success));
	Virt->Size = { 20, 20, 20 };

	arrow = Drawing::MakeArrow({ 1, 1, 1 }, 16, shaderProgram, {"arrow1"}, content, memory);
	arrow->Rotation = Matrix4::CreateRotationX(Pi32 / 2);
	arrow->Position.y = 550;
	arrow->Size.z = 3;
	arrow->Size *= 50;
	Virt->Size *= 20;

	/*Camera = new Drawing::SphericalCamera();
	Camera->SetProjection(Matrix4::CreatePerspective(Pi32 / 2, 16.0f / 9.0f, 1, 1000));
	gameState->Rescalers.RegisterObject(&(globals->Camera->Projection), Matrix4::RescalePerspective);
	Camera->SetLookAtPosition({ 0, 1, 0 });
	Camera->SetCameraPosition({ 0, 2, 3 });
	/*state->globals.Camera.Projection = Matrix4::CreateOrthographic(1280, 720, 0.1f, 1000);
	Camera->Position = { 0, 0, -200 };
	arrow->Size *= 100;
	enterButton->Size *= 100;
	Virt->Size *= 20;
	Virt->Position = { 0, -125, 0 };
	Virt->Pivot = { 0, -10, 0 };*/
	//Camera->Rotate(Quaternion::CreateFromAxisAngle(Vector3::Left(), 0.3f));
	//Camera->SetZoom(3.0f);

	Camera = new Drawing::Camera2D(gameState->WindowSettings.WindowSize, { 0, 0 });
	gameState->Rescalers.RegisterObject(&(Camera->Projection), Matrix4::RescaleOrthographic);
}

void TestScene::HandleInput(GameState *state)
{
	Camera->HandleInput(state->Input);
}

void TestScene::Update(GameState *state)
{
	Camera->Update();
	Virt->Rotation *= Matrix4::CreateRotationY(0.05f);
}

void TestScene::Draw(GameState *state)
{
	//These are static intentionally, it visually shows the reset when the dll reloads
	static real32 sign = 1;
	static real32 colorTest = 0.0f;

	colorTest += 0.003f * sign;
	if (colorTest > 1.0f || colorTest < 0.0f)
		sign *= -1;
	glClearColor(colorTest, colorTest, colorTest, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4 viewProjection = Camera->GetViewProjection();
	Virt->Draw(viewProjection);
	enterButton->Draw(viewProjection);
	arrow->Draw(viewProjection);
}

#endif //HANDMADE_TESTSCENE_CPP