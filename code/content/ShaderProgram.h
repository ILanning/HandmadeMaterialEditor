#ifndef HANDMADE_CONTENT_SHADERPROGRAM_H
#define HANDMADE_CONTENT_SHADERPROGRAM_H

#include "../libraries/glew.h"
#include "ShaderPart.h"
#include "../handmade_typedefs.h"
#include "../handmade_funcdefs.h"
#include "../general/HMPair.h"
#include "../general/HMString.h"

namespace Content
{
	class ShaderProgram
	{
	public:
		GLuint VertShader;
		GLuint FragShader;
		GLuint ID;

		ShaderProgram(GLuint vert, GLuint frag, DebugMessageErrorFunc *messageError) : FragShader(frag), VertShader(vert)
		{
			ID = glCreateProgram();
			glAttachShader(ID, VertShader);
			glAttachShader(ID, FragShader);

			glBindFragDataLocation(ID, 0, "outColor");
			glLinkProgram(ID);

			GLint isLinked;
			glGetProgramiv(ID, GL_LINK_STATUS, &isLinked);
			if (!isLinked)
			{
				//TODO: True diagnostics
				if (messageError)
				{
					messageError("Shader link failed!");
				}
			}
		}
	};
}

#endif //HANDMADE_CONTENT_SHADERPROGRAM_H