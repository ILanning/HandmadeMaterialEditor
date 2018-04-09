#ifndef HANDMADE_TEXTURE2D_H
#define HANDMADE_TEXTURE2D_H

#include "../handmade_typedefs.h"
#include "../libraries/glew.h"
#include "../content/TextureMapType.h"
#include "../content/MTLTextureOptions.h"
#include "../general/StringHelpers.cpp"

namespace Drawing
{
	class Texture2D
	{
		GLuint GLID = 0;

	public:
		char *Name = nullptr;
		int32 NameLength = 0;
		uint8 *Data = nullptr;
		GLint ImageFormat = 0;
		GLsizei Width = 0;
		GLsizei Height = 0;
		GLint WrapStyle = 0;
		GLenum GLFormat = 0;

	private:
		void textureGLSetup()
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

	public:
		Texture2D(uint8 *data, GLsizei width, GLsizei height, GLint imageFormat, GLenum glFormat, GLint wrapStyle, char *name, int32 nameLength = -1)
			:Data(data), Width(width), Height(height), ImageFormat(imageFormat), GLFormat(glFormat), WrapStyle(wrapStyle),
			Name(name), NameLength(nameLength == -1 ? CString::GetLength(name) : nameLength)
		{
			textureGLSetup();
		}

		Texture2D(char *path, char *name = nullptr, int32 nameLength = -1)
		{
			int32 components;
			Data = stbi_load(path, &Width, &Height, &components, 4);
			if (name == nullptr)
			{
				Name = CString::CopySubstring(path, CString::GetLength(path) - 1, &NameLength);
			}
			else
			{
				Name = CString::CopySubstring(name, nameLength);
				NameLength = nameLength;
			}
			WrapStyle = GL_REPEAT;
			ImageFormat = GL_RGBA;
			GLFormat = GL_RGBA;

			textureGLSetup();
		}

		Texture2D(const Content::OBJ::MTLTextureOptions &options, const Content::TextureMapType mapType)
		{
			Name = CString::CopySubstring(options.TexturePath, options.PathLength - 1, &NameLength);

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
			ImageFormat = GL_RGBA;
			GLFormat = GL_RGBA;

			textureGLSetup();
		}

		void Bind(const GLenum textureUnit) const
		{
			glActiveTexture(textureUnit);
			Bind();
		}

		void Bind() const
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
}

#endif //HANDMADE_TEXTURE2D_H