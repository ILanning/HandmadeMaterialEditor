#ifndef HANDMADE_TEXTURE2D_H
#define HANDMADE_TEXTURE2D_H

#include "../handmade_typedefs.h"
#include "../libraries/glew.h"
#include "../libraries/stb_image.h"
#include "../content/TextureMapType.h"
#include "../content/MTLTextureOptions.h"

namespace Drawing
{
	///Represents a typical 2D image, such as the sort found in a png or jpg file.
	class Texture2D
	{
		GLuint GLID = 0;

	public:
		uint8 *Data = nullptr;
		GLint ImageFormat = 0;
		GLsizei Width = 0;
		GLsizei Height = 0;
		GLint WrapStyle = 0;
		GLenum GLFormat = 0;

	private:
		void textureGLSetup();

	public:
		Texture2D(uint8 *data, GLsizei width, GLsizei height, GLint imageFormat, GLenum glFormat, GLint wrapStyle);

		Texture2D(char *path);

		Texture2D(const Content::OBJ::MTLTextureOptions &options, const Content::TextureMapType mapType);

		///Sets this as the active texture within OpenGL
		void Bind(const GLenum textureUnit) const;

		///Sets this as the active texture within OpenGL
		void Bind() const;

		~Texture2D();
	};
}

#endif //HANDMADE_TEXTURE2D_H