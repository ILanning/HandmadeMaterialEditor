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

		virtual ~VertexArray() = 0;
	};

	VertexArray::~VertexArray() {}

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
		void BindShaderVariables(GLuint shaderProgram)
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

		VertexColorTexture &operator[](const int32 index)
		{
			return Vertices[index];
		}

		///Returns the size of the buffer, in bytes.
		int32 GetBufferSize() const
		{
			return sizeof(VertexColorTexture) * VertexCount;
		}

		GLfloat *GetBuffer()
		{
			return (GLfloat *)(void *)Vertices;
		}

		~VertexColorTextureArray()
		{
			delete[] Vertices;
		}
	};

	///A Vertex Array meant for vertices with a position, a normal vector, and a texture coordinate.
	struct VertexNormalTextureArray : VertexArray
	{
		VertexNormalTexture *Vertices;
		uint32 VertexCount;

		VertexNormalTextureArray(VertexNormalTexture *vertices, uint32 vertexCount) : Vertices(vertices), VertexCount(vertexCount) {}

		///Sets up OpenGL to have this sort of vertex work with the passed in shader.
		void BindShaderVariables(GLuint shaderProgram)
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

		VertexNormalTexture &operator[](const int32 index)
		{
			return Vertices[index];
		}

		///Returns the size of the buffer, in bytes.
		int32 GetBufferSize() const
		{
			return sizeof(VertexNormalTexture) * VertexCount;
		}

		///Returns a pointer to the vertex buffer.
		GLfloat *GetBuffer()
		{
			return (GLfloat *)(void *)Vertices;
		}

		~VertexNormalTextureArray()
		{
			delete[] Vertices;
		}
	};
}

#endif //HANDMADE_VERTEXARRAY_H