#ifndef HANDMADE_DEBUGHELPERS
#define HANDMADE_DEBUGHELPERS

#include "../libraries/glew.h"
#include "../handmade_typedefs.h"

internal void DebugOutputGLErrors()
{
	GLenum glError = glGetError();
	while (glError != GL_NO_ERROR)
	{
		char *glErrorString;
		switch (glError)
		{
		case GL_INVALID_ENUM:
		{
			glErrorString = "GL_INVALID_ENUM\n";
		} break;
		case GL_INVALID_VALUE:
		{
			glErrorString = "GL_INVALID_VALUE\n";
		} break;
		case GL_INVALID_OPERATION:
		{
			glErrorString = "GL_INVALID_OPERATION\n";
		} break;
		case GL_STACK_OVERFLOW:
		{
			glErrorString = "GL_STACK_OVERFLOW\n";
		} break;
		case GL_STACK_UNDERFLOW:
		{
			glErrorString = "GL_STACK_UNDERFLOW\n";
		} break;
		case GL_OUT_OF_MEMORY:
		{
			glErrorString = "GL_OUT_OF_MEMORY\n";
		} break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			glErrorString = "GL_INVALID_FRAMEBUFFER_OPERATION\n";
		} break;
		case GL_CONTEXT_LOST:
		{
			glErrorString = "GL_CONTEXT_LOST\n";
		} break;
		default:
		{
			glErrorString = "Unknown OpenGL error!\n";
		} break;
		}
		OutputDebugString(glErrorString);
		glError = glGetError();
	}
}

#endif