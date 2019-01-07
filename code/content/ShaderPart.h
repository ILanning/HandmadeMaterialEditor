#ifndef HANDMADE_CONTENT_SHADERPART_H
#define HANDMADE_CONTENT_SHADERPART_H

#include "../libraries/glew.h"
#include "../handmade_typedefs.h"
#include "../handmade_funcdefs.h"
#include "../general/HMString.h"

namespace Content
{
	struct ShaderPart
	{
		GLuint ID;
		HMString Path;
		HMString Source;

		ShaderPart(HMString& sourcePath, ReadFileFunc *readFile, GLuint shaderType)
		{
			FileData result = readFile(sourcePath.RawCString(), sourcePath.Length(), nullptr);
			ID = glCreateShader(shaderType);
			glShaderSource(ID, 1, &(GLchar *)result.File, NULL);
			glCompileShader(ID);
			Path = sourcePath;
			Source = {(char*)result.File, result.FileSize};
		}


	};
}

#endif //HANDMADE_CONTENT_SHADERPART_H