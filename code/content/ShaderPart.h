#ifndef HANDMADE_CONTENT_SHADERPART_H
#define HANDMADE_CONTENT_SHADERPART_H

#include "../libraries/glew.h"
#include "../handmade_typedefs.h"
#include "../handmade_funcdefs.h"
#include "../general/HMString.h"

namespace Content
{
	///Represents a part of a shader program, such as an individual fragment or vertex shader.
	struct ShaderPart
	{
		GLuint ID;
		HMString Path;
		HMString Source;

		ShaderPart(const HMString& sourcePath, ReadFileFunc *readFile, const GLuint shaderType)
		{
			FileData result = readFile(sourcePath.RawCString(), sourcePath.Length(), nullptr);
			ID = glCreateShader(shaderType);
			GLchar* code = (GLchar *)result.File;
			glShaderSource(ID, 1, &code, NULL);
			glCompileShader(ID);
			Path = sourcePath;
			Source = {(char*)result.File, result.FileSize};
		}
	};
}

#endif //HANDMADE_CONTENT_SHADERPART_H