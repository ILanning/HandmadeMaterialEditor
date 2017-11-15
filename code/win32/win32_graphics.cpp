//Purely for getting Visual Studio Intellisense hints
#ifndef GLEW_IMPORTED

#include "..\libraries\glew.h"
#include "..\libraries\wglew.h"
#define GLEW_IMPORTED 1

#endif
#include "..\handmade_typedefs.h"
#include "..\handmade.h"

#include "win32_debug.cpp"

#include "..\math\Vector2.h"
#include "..\math\Vector3.h"
#include "..\math\Matrix4.h"
#include "..\drawing\GLState.h"
#include "..\drawing\Texture2D.h"
#include "..\drawing\Geometry.h"
#include "..\drawing\Model.h"
#include "..\drawing\Vertex.h"
#include "..\drawing\GeometryHelpers.cpp"

/*internal void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
	// TODO(casey): Bulletproof this.
	// Maybe don't free first, free after, then free first if that fails.

	if (Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}

	Buffer->Width = Width;
	Buffer->Height = Height;

	int BytesPerPixel = 4;
	Buffer->BytesPerPixel = BytesPerPixel;

	// NOTE(casey): When the biHeight field is negative, this is the clue to
	// Windows to treat this bitmap as top-down, not bottom-up, meaning that
	// the first three bytes of the image are the color for the top left pixel
	// in the bitmap, not the bottom left!
	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = Buffer->Width;
	Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32;
	Buffer->Info.bmiHeader.biCompression = BI_RGB;

	// NOTE(casey): Thank you to Chris Hecker of Spy Party fame
	// for clarifying the deal with StretchDIBits and BitBlt!
	// No more DC for us.
	int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	Buffer->Pitch = Width*BytesPerPixel;

	// TODO(casey): Probably clear this to black
}*/

/*internal void Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
	HDC DeviceContext, int WindowWidth, int WindowHeight)
{
	// NOTE(casey): For prototyping purposes, we're going to always blit
	// 1-to-1 pixels to make sure we don't introduce artifacts with
	// stretching while we are learning to code the renderer!
	StretchDIBits(DeviceContext,
		0, 0, Buffer->Width, Buffer->Height,
		0, 0, Buffer->Width, Buffer->Height,
		Buffer->Memory,
		&Buffer->Info,
		DIB_RGB_COLORS, SRCCOPY);
}*/

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
        Texcoord = texcoord;
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

internal void MessageGLErrors()
{
	GLenum glError = glGetError();
	while (glError != GL_NO_ERROR)
	{
		char *glErrorString;
		switch (glError)
		{
		case GL_INVALID_ENUM:
		{
			glErrorString = "GL_INVALID_ENUM";
		} break;
		case GL_INVALID_VALUE:
		{
			glErrorString = "GL_INVALID_VALUE";
		} break;
		case GL_INVALID_OPERATION:
		{
			glErrorString = "GL_INVALID_OPERATION";
		} break;
		case GL_STACK_OVERFLOW:
		{
			glErrorString = "GL_STACK_OVERFLOW";
		} break;
		case GL_STACK_UNDERFLOW:
		{
			glErrorString = "GL_STACK_UNDERFLOW";
		} break;
		case GL_OUT_OF_MEMORY:
		{
			glErrorString = "GL_OUT_OF_MEMORY";
		} break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			glErrorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
		} break;
		case GL_CONTEXT_LOST:
		{
			glErrorString = "GL_CONTEXT_LOST";
		} break;
		default:
		{
			glErrorString = "Unknown OpenGL error!";
		} break;
		}
		MessageBoxA(0, glErrorString, "GL Error!", 0);
		glError = glGetError();
	}
}

static Model *arrow;
static Model *enterButton;
static Model *enter2;
static Model *Virt;

void BuildTestObjects(GLuint shaderProgram)
{
	int32 width, height, components;
	uint8 *image = stbi_load("EnterButton.png", &width, &height, &components, 3);
	real32 imageRatio = ((real32)width) / height;

	Texture2D *enterTexture = new Texture2D(image, width, height, GL_RGB, GL_RGBA);

	VertexColorTexture *vertices = new VertexColorTexture[4];
	//Position                   Color                   Texcoords
	vertices[0] = { -0.5f * imageRatio,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // Top-left
	vertices[1] = { 0.5f * imageRatio,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }; // Top-right
	vertices[2] = { 0.5f * imageRatio, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }; // Bottom-right
	vertices[3] = { -0.5f * imageRatio, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }; // Bottom-left


	GLuint *elements = new GLuint[6]{
		0, 1, 2,
		2, 3, 0
	};

	thread_context dummy = {};
	debug_read_file_result file = DEBUGPlatformReadEntireFile(&dummy, "Assets/virt/Virt.obj");

	int32 virtWidth, virtHeight, virtComponents;
	uint8 *virtImage = stbi_load("Assets/virt/VirtTextureMapFinal.png", &virtWidth, &virtHeight, &virtComponents, 4);
	Texture2D *virtTex = new Texture2D(virtImage, virtWidth, virtHeight, GL_RGBA, GL_RGBA);
	Geometry *virtMesh = ParseOBJ((char *)file.Contents, file.ContentsSize, shaderProgram, virtTex);
	Virt = new Model(virtMesh);

	Geometry *enterMesh = new Geometry(new VertexColorTextureArray(vertices, 4), elements, 6, shaderProgram, enterTexture);
	enterButton = new Model(enterMesh, { 0, 0, 0 }, { (real32)1, (real32)1, 1 });
	enter2 = new Model(enterMesh, { 0, 1, 0 }, { (real32)1, (real32)1, 1 });
	arrow = MakeArrow({ 1, 1, 1 }, 16, shaderProgram);

	arrow->Rotation = Matrix4::CreateRotationX(Pi32 / 2);
	arrow->Position.y = 3;
	arrow->Size.z = 3;
}

GLuint BuildShaderProgram(const GLchar *vertexSource, const GLchar *fragmentSource)
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
		MessageBoxA(0, "Shader link failed!", 0, 0);
	}

	return shaderProgram;
}

internal GLState* PrepareScene()
{
	GLState *glState = new GLState();
		
	glEnable(GL_DEPTH_TEST);

	GLuint shaderProgram = BuildShaderProgram(vertexSourceCode, fragmentSourceCode);
	glState->ShaderProgram = shaderProgram;

	glUseProgram(shaderProgram);

	BuildTestObjects(shaderProgram);

	//glState->SetProjection(Matrix4::CreatePerspective(Pi32 / 2, 16.0f / 9.0f, 1, 100));
	glState->SetProjection(Matrix4::CreateOrthographic(1280, 720, 0.1f, 1000));
	arrow->Size *= 100;
	enterButton->Size *= 100;
	enter2->Size *= 100;
	enter2->Position *= 100;
	Virt->Size *= 20;
	Virt->Position = { 0, -275, 0 };
	glState->SetView(Matrix4::CreateTranslation({ 0, 0, -10 }));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	MessageGLErrors();

	return glState;
}

void GLRender(GLState *glState, game_input *input)
{

	Vector3 rotateDirection = {};
	if (input->Controllers[0].MoveDown.EndedDown)
	{
		rotateDirection.x++;
	}
	if (input->Controllers[0].MoveUp.EndedDown)
	{
		rotateDirection.x--;
	}
	if (input->Controllers[0].MoveLeft.EndedDown)
	{
		rotateDirection.y++;
	}
	if (input->Controllers[0].MoveRight.EndedDown)
	{
		rotateDirection.y--;
	}
	if (input->Controllers[0].RightShoulder.EndedDown)
	{
		rotateDirection.z++;
	}
	if (input->Controllers[0].LeftShoulder.EndedDown)
	{
		rotateDirection.z--;
	}

	real32 rotateSpeed = 0.05f;
	if (rotateDirection.MagnitudeSquared() != 0)
	{
		rotateDirection = rotateDirection.Normalize() * rotateSpeed;
	}
	Virt->Rotation *= Matrix4::CreateRotation(rotateDirection);


	Vector3 moveDirection = {};
	if (input->Controllers[0].ActionRight.EndedDown)
	{
		moveDirection.x++;
	}
	if (input->Controllers[0].ActionLeft.EndedDown)
	{
		moveDirection.x--;
	}
	if (input->Controllers[0].ActionUp.EndedDown)
	{
		moveDirection.y++;
	}
	if (input->Controllers[0].ActionDown.EndedDown)
	{
		moveDirection.y--;
	}
	if (input->Controllers[0].Start.EndedDown)
	{
		moveDirection.z++;
	}
	if (input->Controllers[0].Back.EndedDown)
	{
		moveDirection.z--;
	}

	real32 moveSpeed = 0.03f;
	if (rotateDirection.MagnitudeSquared() != 0)
	{
		moveDirection = moveDirection.Normalize() * moveSpeed;
	}
	arrow->Position += moveDirection;

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Virt->Draw(glState->View, glState->Projection);
	//enterButton->Draw(glState->View, glState->Projection);
	//enter2->Draw(glState->View, glState->Projection);
	//arrow->Draw(glState->View, glState->Projection);
	
	SwapBuffers(GlobalDeviceContext);
}

#if 0

internal void
Win32DebugDrawVertical(win32_offscreen_buffer *Backbuffer,
	int X, int Top, int Bottom, uint32 Color)
{
	if (Top <= 0)
	{
		Top = 0;
	}

	if (Bottom > Backbuffer->Height)
	{
		Bottom = Backbuffer->Height;
	}

	if ((X >= 0) && (X < Backbuffer->Width))
	{
		uint8 *Pixel = ((uint8 *)Backbuffer->Memory +
			X*Backbuffer->BytesPerPixel +
			Top*Backbuffer->Pitch);
		for (int Y = Top;
			Y < Bottom;
			++Y)
		{
			*(uint32 *)Pixel = Color;
			Pixel += Backbuffer->Pitch;
		}
	}
}

inline void
Win32DrawSoundBufferMarker(win32_offscreen_buffer *Backbuffer,
	win32_sound_output *SoundOutput,
	real32 C, int PadX, int Top, int Bottom,
	DWORD Value, uint32 Color)
{
	real32 XReal32 = (C * (real32)Value);
	int X = PadX + (int)XReal32;
	Win32DebugDrawVertical(Backbuffer, X, Top, Bottom, Color);
}

internal void
Win32DebugSyncDisplay(win32_offscreen_buffer *Backbuffer,
	int MarkerCount, win32_debug_time_marker *Markers,
	int CurrentMarkerIndex,
	win32_sound_output *SoundOutput, real32 TargetSecondsPerFrame)
{
	int PadX = 16;
	int PadY = 16;

	int LineHeight = 64;

	real32 C = (real32)(Backbuffer->Width - 2 * PadX) / (real32)SoundOutput->SecondaryBufferSize;
	for (int MarkerIndex = 0;
		MarkerIndex < MarkerCount;
		++MarkerIndex)
	{
		win32_debug_time_marker *ThisMarker = &Markers[MarkerIndex];
		Assert(ThisMarker->OutputPlayCursor < SoundOutput->SecondaryBufferSize);
		Assert(ThisMarker->OutputWriteCursor < SoundOutput->SecondaryBufferSize);
		Assert(ThisMarker->OutputLocation < SoundOutput->SecondaryBufferSize);
		Assert(ThisMarker->OutputByteCount < SoundOutput->SecondaryBufferSize);
		Assert(ThisMarker->FlipPlayCursor < SoundOutput->SecondaryBufferSize);
		Assert(ThisMarker->FlipWriteCursor < SoundOutput->SecondaryBufferSize);

		DWORD PlayColor = 0xFFFFFFFF;
		DWORD WriteColor = 0xFFFF0000;
		DWORD ExpectedFlipColor = 0xFFFFFF00;
		DWORD PlayWindowColor = 0xFFFF00FF;

		int Top = PadY;
		int Bottom = PadY + LineHeight;
		if (MarkerIndex == CurrentMarkerIndex)
		{
			Top += LineHeight + PadY;
			Bottom += LineHeight + PadY;

			int FirstTop = Top;

			Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->OutputPlayCursor, PlayColor);
			Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->OutputWriteCursor, WriteColor);

			Top += LineHeight + PadY;
			Bottom += LineHeight + PadY;

			Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->OutputLocation, PlayColor);
			Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->OutputLocation + ThisMarker->OutputByteCount, WriteColor);

			Top += LineHeight + PadY;
			Bottom += LineHeight + PadY;

			Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, FirstTop, Bottom, ThisMarker->ExpectedFlipPlayCursor, ExpectedFlipColor);
		}

		Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->FlipPlayCursor, PlayColor);
		Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->FlipPlayCursor + 480 * SoundOutput->BytesPerSample, PlayWindowColor);
		Win32DrawSoundBufferMarker(Backbuffer, SoundOutput, C, PadX, Top, Bottom, ThisMarker->FlipWriteCursor, WriteColor);
	}
}

#endif