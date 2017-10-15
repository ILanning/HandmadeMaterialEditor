//Purely for getting Visual Studio Intellisense hints
#ifndef GLEW_IMPORTED

#include "..\libraries\glew.h"
#include "..\libraries\wglew.h"
#define GLEW_IMPORTED 1

#endif

#include "..\math\Vector2.h"

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
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
}

internal void
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
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
}

struct GLObjects
{
	
};

const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 Texcoord;
    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
		vec4 texel = texture(tex, Texcoord);
		if(texel.a < 1)
		{
			discard;
		}
        outColor = texel * vec4(Color, 1.0);
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

internal void PrepareScene()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	GLfloat vertices[] = {
		//  Position      Color             Texcoords
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

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
	glUseProgram(shaderProgram);

	GLint isLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		//TODO(Ian): True diagnostics
		MessageBoxA(0, "Shader link failed!", 0, 0);
	}

	int vertexSize = 7 * sizeof(GLfloat);
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, vertexSize, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(2 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(5 * sizeof(GLfloat)));

	// Load texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	int width, height, components;
	//unsigned char* image = SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);
	uint8 *image = stbi_load("EnterButton.png", &width, &height, &components, 4);
	/*uint8 image[] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
		0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff};
	width = 2;
	height = 2;*/

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	delete image;
	//SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	MessageGLErrors();
}

void GLRender()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glViewport(0, 0, 1280, 720);
	
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