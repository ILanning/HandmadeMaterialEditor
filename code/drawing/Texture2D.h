#ifndef HANDMADE_TEXTURE2D
#define HANDMADE_TEXTURE2D

#include "../handmade_typedefs.h"
#include "../libraries/glew.h"
#include "../content/TextureMapType.h"
#include "../content/MTLTextureOptions.h"

struct Texture2D
{
	uint8 *Data;
	GLint ImageFormat;
	GLsizei Width;
	GLsizei Height;
	GLint WrapStyle;
	GLenum GLFormat;
	GLuint GLID;

	Texture2D(uint8 *data, GLsizei width, GLsizei height, GLint imageFormat, GLenum glFormat)
		:Data(data), Width(width), Height(height), ImageFormat(imageFormat), GLFormat(glFormat)
	{
		glGenTextures(1, &GLID);
		glBindTexture(GL_TEXTURE_2D, GLID);
		glTexImage2D(GL_TEXTURE_2D, 0, ImageFormat, Width, Height, 0, GLFormat, GL_UNSIGNED_BYTE, Data);
		WrapStyle = GL_REPEAT;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapStyle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapStyle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D(Content::OBJ::MTLTextureOptions options, Content::TextureMapType mapType)
	{
		glGenTextures(1, &GLID);
		glBindTexture(GL_TEXTURE_2D, GLID); 
		int32 components;
		Data = stbi_load(options.TexturePath, &Width, &Height, &components, 4);
		if (options.Clamp)
		{
			WrapStyle = GL_CLAMP_TO_EDGE;
		}
		else
		{
			WrapStyle = GL_REPEAT;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, ImageFormat, Width, Height, 0, GLFormat, GL_UNSIGNED_BYTE, Data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapStyle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapStyle);
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
		Data = nullptr;
		glDeleteTextures(1, &GLID);
	}
};

#endif