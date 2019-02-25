#ifndef HANDMADE_DEBUGHELPERS_H
#define HANDMADE_DEBUGHELPERS_H

#include "../libraries/glew.h"
#include "../handmade_typedefs.h"
#include "../handmade_debugfuncs.h"

//This function is handy for debugging OpenGL errors, so we want to keep it but we don't always need it referenced by something
#pragma warning( push )
#pragma warning( disable : 4505)

inline internal void DebugOutputGLErrors(DebugMessageErrorFunc *messageError)
{
	GLenum glError = glGetError();
	while (glError != GL_NO_ERROR)
	{
		switch (glError)
		{
		case GL_INVALID_ENUM:
		{
			messageError("GL_INVALID_ENUM\n");
		} break;
		case GL_INVALID_VALUE:
		{
			messageError("GL_INVALID_VALUE\n");
		} break;
		case GL_INVALID_OPERATION:
		{
			messageError("GL_INVALID_OPERATION\n");
		} break;
		case GL_STACK_OVERFLOW:
		{
			messageError("GL_STACK_OVERFLOW\n");
		} break;
		case GL_STACK_UNDERFLOW:
		{
			messageError("GL_STACK_UNDERFLOW\n");
		} break;
		case GL_OUT_OF_MEMORY:
		{
			messageError("GL_OUT_OF_MEMORY\n");
		} break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			messageError("GL_INVALID_FRAMEBUFFER_OPERATION\n");
		} break;
		case GL_CONTEXT_LOST:
		{
			messageError("GL_CONTEXT_LOST\n");
		} break;
		default:
		{
			messageError("Unknown OpenGL error!\n");
		} break;
		}
		glError = glGetError();
	}
}
#pragma warning( pop ) 

#endif //HANDMADE_DEBUGHELPERS_H