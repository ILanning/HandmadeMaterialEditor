#ifndef HANDMADE_TESTSCENE_CPP
#define HANDMADE_TESTSCENE_CPP

#include "TestScene.h"

#include "GameState.h"
#include "drawing\GLState.h"
#include "drawing\Texture2D.h"
#include "drawing\Geometry.h"
#include "drawing\Vertex.h"
#include "math\Vector2.cpp"
#include "math\Vector3.cpp"
#include "math\Matrix4.cpp"
#include "drawing\GeometryHelpers.cpp"
#include "content\OBJLoader.cpp"
#include "general\StringHelpers.cpp"

GLuint TestScene::BuildShaderProgram(char *vertexSourcePath, char *fragmentSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError)
{
	FileData vertexResult = readFile(vertexSourcePath, CString::GetLength(vertexSourcePath), nullptr);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(GLchar *)vertexResult.File, NULL);
	glCompileShader(vertexShader);

	FileData fragResult = readFile(fragmentSourcePath, CString::GetLength(vertexSourcePath), nullptr);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &(GLchar *)fragResult.File, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	GLint isLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		//TODO(Ian): True diagnostics
		messageError("Shader link failed!");
	}

	return shaderProgram;
}

void TestScene::Initialize(ReadFileFunc *readFile, DebugMessageErrorFunc *messageError, GameState *gameState)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //Uncomment to get a wireframe view

	GLuint shaderProgram = BuildShaderProgram("Shaders\\Basic.vert", "Shaders\\Basic.frag", readFile, messageError);

	glUseProgram(shaderProgram);

	Drawing::Material *enterMaterial = new Drawing::Material("EnterMaterial", 14, new Drawing::Texture2D("EnterButton.png"));
	enterButton = new Drawing::Sprite(enterMaterial, shaderProgram);

	//globals->enterButton->SetSampleArea({ 75, 0, 40, 66 });

	Drawing::Geometry *virtMesh = Content::ParseOBJ("Assets/virt/virt.obj", 21, shaderProgram, readFile);
	//DebugOutputGLErrors(messageError);
	Virt = new Drawing::Model(virtMesh);
	Virt->Size = { 20, 20, 20 };

	arrow = Drawing::MakeArrow({ 1, 1, 1 }, 16, shaderProgram);
	arrow->Rotation = Matrix4::CreateRotationX(Pi32 / 2);

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

	arrow->Position.y = 550;
	arrow->Size.z = 3;
	arrow->Size *= 50;
	//Virt->Size *= 20;
}

void TestScene::HandleInput(GameState *state)
{
	Camera->HandleInput(state->Input);
}

void TestScene::Update(GameState *state)
{
	Camera->Update();
	Virt->Rotation *= Matrix4::CreateRotationY(0.05f);
	//Virt->Rotation *= Matrix4::CreateRotationX(0.10f);
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