#ifndef HANDMADE_RENDER
#define HANDMADE_RENDER

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
#include "drawing\Vertex.h"
#include "drawing\cameras\Camera.h"
#include "drawing\cameras\FreeRotateCamera.h"
#include "drawing\cameras\SphericalCamera.h"
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

void BuildTestObjects(GLuint shaderProgram,  ReadFileFunc *readFile, DebugMessageErrorFunc *messageError, GameState *gameState)
{
	TestGlobals *globals = &(gameState->globals);

	int32 width, height, components;
	uint8 *image = stbi_load("EnterButton.png", &width, &height, &components, 4);
	real32 imageRatio = ((real32)width) / height;

	Texture2D *enterTexture = new Texture2D(image, width, height, GL_RGBA, GL_RGBA, CString::CopySubstring("EnterButton.png", 15), 16);

	VertexColorTexture *vertices = new VertexColorTexture[4];
	//Position                   Color                   Texcoords
	vertices[0] = { -0.5f * imageRatio,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // Top-left
	vertices[1] = { 0.5f * imageRatio,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }; // Top-right
	vertices[2] = { 0.5f * imageRatio, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }; // Bottom-right
	vertices[3] = { -0.5f * imageRatio, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }; // Bottom-left


	GLuint *elements = new GLuint[6]{
		2, 1, 0,
		0, 3, 2
	};

	Geometry *virtMesh = Content::ParseOBJ("Assets/virt/Virt.obj", 21, shaderProgram, readFile);
	//DebugOutputGLErrors(messageError);
	globals->Virt = new Model(virtMesh);

	Material *enterMaterial = new Material("EnterMaterial", 14, enterTexture);
	Mesh *enterMesh = new Mesh(new VertexColorTextureArray(vertices, 4), elements, 6, shaderProgram, enterMaterial);
	Geometry *enterGeo = new Geometry(enterMesh, 1);
	globals->enterButton = new Model(enterGeo, { 0, 0, 0 }, { (real32)1, (real32)1, 1 });
	globals->enter2 = new Model(enterGeo, { 0, 1, 0 }, { (real32)1, (real32)1, 1 });

	globals->arrow = MakeArrow({ 1, 1, 1 }, 16, shaderProgram);
	globals->arrow->Rotation = Matrix4::CreateRotationX(Pi32 / 2);

	globals->Camera = new Drawing::SphericalCamera();
	globals->Camera->SetProjection(Matrix4::CreatePerspective(Pi32 / 2, 16.0f / 9.0f, 1, 1000));
	gameState->Rescalers.RegisterObject(&(globals->Camera->projection), Matrix4::RescalePerspective);
	globals->Camera->SetLookAtPosition({ 0, 1, 0 });
	globals->Camera->SetCameraPosition({ 0, 2, 3 });
	/*state->globals.Camera.Projection = Matrix4::CreateOrthographic(1280, 720, 0.1f, 1000);
	globals->Camera->Position = { 0, 0, -200 };
	globals->arrow->Size *= 100;
	globals->enterButton->Size *= 100;
	globals->enter2->Size *= 100;
	globals->enter2->Position *= 100;
	globals->Virt->Size *= 20;
	globals->Virt->Position = { 0, -125, 0 };
	globals->Virt->Pivot = { 0, -10, 0 };*/
	//globals->Camera->Rotate(Quaternion::CreateFromAxisAngle(Vector3::Left(), 0.3f));
	globals->Camera->SetZoom(3.0f);
	globals->arrow->Position.y = 3;
	globals->arrow->Size.z = 3;
	globals->Virt->Size /= 10;
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

extern "C" GAME_INITIALIZE(GameInitialize)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLuint shaderProgram = BuildShaderProgram(vertexSourceCode, fragmentSourceCode, memory->DEBUGMessageError);

	glUseProgram(shaderProgram);

	GameState *state = (GameState*)memory->PermanentStorage;

	BuildTestObjects(shaderProgram, memory->ReadEntireFile, memory->DEBUGMessageError, state);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
	state->globals.enter2->Draw(viewProjection);
	state->globals.arrow->Draw(viewProjection);
}

#endif