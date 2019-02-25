#ifndef HANDMADE_VERTEXARRAY_H
#define HANDMADE_VERTEXARRAY_H

#include "../handmade_typedefs.h"
#include "Vertex.h"
#include "../libraries/glew.h"

//TODO:  Define a VertexInfo class with one child class for each type of vertex, to 
//            better enable functions that can handle many types of vertices
namespace Drawing
{
	///An abstract structure for arrays that contain vertices.
	struct VertexArray
	{
		virtual void BindShaderVariables(GLuint shaderProgram) = 0;
		virtual int32 GetBufferSize() const = 0;
		virtual GLfloat *GetBuffer() = 0;

		//virtual ~VertexArray() = 0;
	};
}

#endif //HANDMADE_VERTEXARRAY_H