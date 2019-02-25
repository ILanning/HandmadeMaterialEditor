#ifndef HANDMADE_VERTEXNORMALTEXTUREARRAY_H
#define HANDMADE_VERTEXNORMALTEXTUREARRAY_H

#include "../handmade_typedefs.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "../libraries/glew.h"

namespace Drawing
{
	///A Vertex Array meant for vertices with a position, a normal vector, and a texture coordinate.
	struct VertexNormalTextureArray : VertexArray
	{
		VertexNormalTexture *Vertices;
		uint32 VertexCount;

		VertexNormalTextureArray(VertexNormalTexture *vertices, uint32 vertexCount) : Vertices(vertices), VertexCount(vertexCount) {}

		///Sets up OpenGL to have this sort of vertex work with the passed in shader.
		void BindShaderVariables(GLuint shaderProgram);

		inline VertexNormalTexture &operator[](const int32 index)
		{
			return Vertices[index];
		}

		///Returns the size of the buffer, in bytes.
		inline int32 GetBufferSize() const
		{
			return sizeof(VertexNormalTexture) * VertexCount;
		}

		///Returns a pointer to the vertex buffer.
		inline GLfloat *GetBuffer()
		{
			return (GLfloat *)(void *)Vertices;
		}

		inline ~VertexNormalTextureArray()
		{
			delete[] Vertices;
		}
	};
}

#endif //HANDMADE_VERTEXNORMALTEXTUREARRAY_H