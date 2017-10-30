#ifndef HANDMADE_TEXTURE2D
#define HANDMADE_TEXTURE2D

#include "../handmade_typedefs.h"
#include "../libraries/glew.h"

struct Texture2D
{
	uint8 *Data;
	GLint ImageFormat;
	GLsizei Width;
	GLsizei Height;
	GLenum GLFormat;
	GLuint GLID;

	Texture2D(uint8 *data, GLsizei width, GLsizei height, GLint imageFormat, GLenum glFormat)
		:Data(data), Width(width), Height(height), ImageFormat(imageFormat), GLFormat(glFormat)
	{
		glGenTextures(1, &GLID);
		glBindTexture(GL_TEXTURE_2D, GLID);
		glTexImage2D(GL_TEXTURE_2D, 0, ImageFormat, Width, Height, 0, GLFormat, GL_UNSIGNED_BYTE, Data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Bind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		Bind();
	}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, GLID);
	}

	~Texture2D()
	{
		delete[] Data;
		glDeleteTextures(1, &GLID);
	}
};

#endif