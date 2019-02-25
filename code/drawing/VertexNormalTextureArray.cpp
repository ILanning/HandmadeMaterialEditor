#ifndef HANDMADE_VERTEXNORMALTEXTUREARRAY_CPP
#define HANDMADE_VERTEXNORMALTEXTUREARRAY_CPP

#include "../handmade_typedefs.h"
#include "Vertex.h"
#include "VertexNormalTextureArray.h"
#include "../libraries/glew.h"

namespace Drawing
{
	void VertexNormalTextureArray::BindShaderVariables(GLuint shaderProgram)
	{
		int32 vertexSize = sizeof(VertexNormalTexture);
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		GLint colAttrib = glGetAttribLocation(shaderProgram, "normal");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(GLfloat)));
	}
}

#endif //HANDMADE_VERTEXNORMALTEXTUREARRAY_CPP
