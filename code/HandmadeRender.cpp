#ifndef HANDMADE_RENDER_CPP
#define HANDMADE_RENDER_CPP

//Purely for getting Visual Studio Intellisense hints
#ifndef GLEW_IMPORTED

#include "libraries\glew.h"
#define GLEW_IMPORTED 1

#endif

#include "handmade_typedefs.h"
#include "math\Vector2.cpp"
#include "math\Vector3.cpp"
#include "math\Matrix4.cpp"
#include "drawing\GLState.h"
#include "drawing\Texture2D.h"
#include "drawing\Geometry.h"
#include "drawing\Model.h"
#include "drawing\Sprite.h"
#include "drawing\Vertex.h"
#include "drawing\cameras\Camera.h"
#include "drawing\cameras\FreeRotateCamera.h"
#include "drawing\cameras\SphericalCamera.h"
#include "drawing\cameras\Camera2D.h"
#include "drawing\GeometryHelpers.cpp"
#include "content\OBJLoader.cpp"
#include "general\StringHelpers.cpp"

const GLchar *vertexSourceCode = R"glsl(
    #version 150 core
    in vec3 position;
    in vec3 color;
    in vec2 texcoord;
	uniform mat4 mvp;
	uniform vec3 meshColor;
    out vec3 Color;
    out vec2 Texcoord;
    void main()
    {
        Color = meshColor;
        Texcoord = vec2(texcoord.x, -texcoord.y);
        gl_Position = mvp * vec4(position, 1.0);
    }
)glsl";

const GLchar *fragmentSourceCode = R"glsl(
    #version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
		vec4 texColor = texture(tex, Texcoord);
		if(texColor.w < 0.5)
		{
			discard;
		}
        outColor = texColor * vec4(Color, 1.0);
    }
)glsl";

void BuildTestObjects(GLuint shaderProgram, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError, GameState *gameState)
{
	TestGlobals *globals = &(gameState->globals);

	int32 width, height, components;
	uint8 *image = stbi_load("EnterButton.png", &width, &height, &components, 4);
	real32 imageRatio = ((real32)width) / height;

	Drawing::Material *enterMaterial = new Drawing::Material("EnterMaterial", 14, new Drawing::Texture2D("EnterButton.png"));
	globals->enterButton = new Drawing::Sprite({ 0, 0 }, 0, { 1, 1 }, 0, enterMaterial, shaderProgram);

	Drawing::Geometry *virtMesh = Content::ParseOBJ("Assets/virt/Virt.obj", 21, shaderProgram, readFile);
	//DebugOutputGLErrors(messageError);
	globals->Virt = new Drawing::Model(virtMesh);

	globals->arrow = Drawing::MakeArrow({ 1, 1, 1 }, 16, shaderProgram);
	globals->arrow->Rotation = Matrix4::CreateRotationX(Pi32 / 2);
	

	/*globals->Camera = new Drawing::SphericalCamera();
	globals->Camera->SetProjection(Matrix4::CreatePerspective(Pi32 / 2, 16.0f / 9.0f, 1, 1000));
	gameState->Rescalers.RegisterObject(&(globals->Camera->Projection), Matrix4::RescalePerspective);
	globals->Camera->SetLookAtPosition({ 0, 1, 0 });
	globals->Camera->SetCameraPosition({ 0, 2, 3 });
	/*state->globals.Camera.Projection = Matrix4::CreateOrthographic(1280, 720, 0.1f, 1000);
	globals->Camera->Position = { 0, 0, -200 };
	globals->arrow->Size *= 100;
	globals->enterButton->Size *= 100;
	globals->Virt->Size *= 20;
	globals->Virt->Position = { 0, -125, 0 };
	globals->Virt->Pivot = { 0, -10, 0 };*/
	//globals->Camera->Rotate(Quaternion::CreateFromAxisAngle(Vector3::Left(), 0.3f));
	//globals->Camera->SetZoom(3.0f);

	globals->Camera = new Drawing::Camera2D(gameState->WindowSettings.WindowSize, { 0, 0 });
	gameState->Rescalers.RegisterObject(&(globals->Camera->Projection), Matrix4::RescaleOrthographic);

	globals->arrow->Position.y = 550;
	globals->arrow->Size.z = 3;
	globals->arrow->Size *= 50;
	globals->Virt->Size *= 20;
}

GLuint BuildShaderProgram(const GLchar *vertexSource, const GLchar *fragmentSource, DebugMessageErrorFunc *messageError)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
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

void RenderScene(GameState *state)
{
	static real32 sign = 1;
	static real32 colorTest = 0.0f;
	colorTest += 0.003f * sign;
	if (colorTest > 1.0f || colorTest < 0.0f)
		sign *= -1;
	glClearColor(colorTest, colorTest, colorTest, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4 viewProjection = state->globals.Camera->GetViewProjection();
	state->globals.Virt->Draw(viewProjection);
	state->globals.enterButton->Draw(viewProjection);
	state->globals.arrow->Draw(viewProjection);
}

#endif //HANDMADE_RENDER_CPP