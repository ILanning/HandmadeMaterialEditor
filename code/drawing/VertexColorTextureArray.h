#ifndef HANDMADE_VERTEXCOLORTEXTUREARRAY_H
#define HANDMADE_VERTEXCOLORTEXTUREARRAY_H

#include "../handmade_typedefs.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "../libraries/glew.h"

namespace Drawing
{
	//CONSIDER: This class would work well as a template/macro/code generation thing
	///A Vertex Array meant for vertices with a position, an RGB color, and a texture coordinate.
	struct VertexColorTextureArray : VertexArray
	{
		VertexColorTexture *Vertices;
		uint32 VertexCount;

		VertexColorTextureArray(VertexColorTexture *vertices, uint32 vertexCount) : Vertices(vertices), VertexCount(vertexCount) {}

		//ASK:  This modifies the currently set VAO, but does not require that that VAO be passed in.
		//	    How should I best signify the global state that this requires, since modifying the current VAO myself here would cause other issues?

		///Sets up OpenGL to have this sort of vertex work with the passed in shader.
		void BindShaderVariables(GLuint shaderProgram);

		inline VertexColorTexture &operator[](const int32 index)
		{
			return Vertices[index];
		}

		///Returns the size of the buffer, in bytes.
		inline int32 GetBufferSize() const
		{
			return sizeof(VertexColorTexture) * VertexCount;
		}

		inline GLfloat *GetBuffer()
		{
			return (GLfloat *)(void *)Vertices;
		}

		inline ~VertexColorTextureArray()
		{
			delete[] Vertices;
		}
	};
}

#endif //HANDMADE_VERTEXCOLORTEXTUREARRAY_H