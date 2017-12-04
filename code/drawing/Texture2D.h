#ifndef HANDMADE_TEXTURE2D
#define HANDMADE_TEXTURE2D

#include "../handmade_typedefs.h"
#include "../libraries/glew.h"
#include "../content/TextureMapType.h"
#include "../content/MTLTextureOptions.h"
#include "../general/StringHelpers.cpp"

struct Texture2D
{
	char *Name = nullptr;
	int32 NameLength = 0;
	uint8 *Data = nullptr;
	GLint ImageFormat = 0;
	GLsizei Width = 0;
	GLsizei Height = 0;
	GLint WrapStyle = 0;
	GLenum GLFormat = 0;
	GLuint GLID = 0;

	Texture2D(uint8 *data, GLsizei width, GLsizei height, GLint imageFormat, GLenum glFormat, char *name, int32 nameLength = -1)
		:Data(data), Width(width), Height(height), ImageFormat(imageFormat), GLFormat(glFormat), 
		 Name(name), NameLength(nameLength == -1 ? CString::GetLength(name) : nameLength)
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
		Name = CString::CopySubstring(options.TexturePath, options.PathLength - 1, &NameLength);

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

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
		delete[] Name;
		Name = nullptr;
		delete[] Data;
		Data = nullptr;
		glDeleteTextures(1, &GLID);
	}
};

#endif