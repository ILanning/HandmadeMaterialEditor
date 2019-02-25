#ifndef HANDMADE_TEXTURE2D_CPP
#define HANDMADE_TEXTURE2D_CPP

#include "Texture2D.h"
#include "../handmade_typedefs.h"
#include "../libraries/glew.h"
#include "../libraries/stb_image.h"
#include "../content/TextureMapType.h"
#include "../content/MTLTextureOptions.h"
#include "../general/HMString.h"
#include "../general/StringHelpers.h"

namespace Drawing
{
	void Texture2D::textureGLSetup()
	{
		glGenTextures(1, &GLID);
		glBindTexture(GL_TEXTURE_2D, GLID);
		glTexImage2D(GL_TEXTURE_2D, 0, ImageFormat, Width, Height, 0, GLFormat, GL_UNSIGNED_BYTE, Data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapStyle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapStyle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::Texture2D(uint8 *data, GLsizei width, GLsizei height, GLint imageFormat, GLenum glFormat, GLint wrapStyle)
		:Data(data), Width(width), Height(height), ImageFormat(imageFormat), GLFormat(glFormat), WrapStyle(wrapStyle)
	{
		textureGLSetup();
	}

	Texture2D::Texture2D(char *path)
	{
		int32 components;
		Data = stbi_load(path, &Width, &Height, &components, 4);
		WrapStyle = GL_REPEAT;
		ImageFormat = GL_RGBA;
		GLFormat = GL_RGBA;

		textureGLSetup();
	}

	Texture2D::Texture2D(const Content::OBJ::MTLTextureOptions &options, const Content::TextureMapType mapType)
	{
		int32 components;
		Data = stbi_load(options.Path.RawCString(), &Width, &Height, &components, 4);
		if (options.Clamp)
		{
			WrapStyle = GL_CLAMP_TO_EDGE;
		}
		else
		{
			WrapStyle = GL_REPEAT;
		}
		ImageFormat = GL_RGBA;
		GLFormat = GL_RGBA;

		textureGLSetup();
	}

	///Sets this as the active texture within OpenGL
	void Texture2D::Bind(const GLenum textureUnit) const
	{
		glActiveTexture(textureUnit);
		Bind();
	}

	///Sets this as the active texture within OpenGL
	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, GLID);
	}

	Texture2D::~Texture2D()
	{
		delete[] Data;
		Data = nullptr;
		glDeleteTextures(1, &GLID);
	}
}

#endif //HANDMADE_TEXTURE2D_CPP