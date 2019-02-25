#ifndef HANDMADE_VERTEXCOLORTEXTUREARRAY_CPP
#define HANDMADE_VERTEXCOLORTEXTUREARRAY_CPP

#include "../handmade_typedefs.h"
#include "Vertex.h"
#include "VertexColorTextureArray.h"
#include "../libraries/glew.h"

namespace Drawing
{
	void VertexColorTextureArray::BindShaderVariables(GLuint shaderProgram)
	{
		//TODO: Figure out why this function is causing a GL_INVALID_VALUE error
		int32 vertexSize = sizeof(VertexColorTexture);
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(GLfloat)));
	}
}

#endif //HANDMADE_VERTEXCOLORTEXTUREARRAY_CPP