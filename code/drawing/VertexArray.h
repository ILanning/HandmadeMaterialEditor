#ifndef HANDMADE_VERTEXARRAY_H
#define HANDMADE_VERTEXARRAY_H

#include "../handmade_typedefs.h"
#include "Vertex.h"
#include "../libraries/glew.h"

//TODO(Ian):  Define a VertexInfo class with one child class for each type of vertex, to 
//            better enable functions that can handle many types of vertices
namespace Drawing
{
	struct VertexArray
	{
		virtual void BindShaderVariables(GLuint shaderProgram) = 0;
		virtual int32 GetBufferSize() = 0;
		virtual GLfloat *GetBuffer() = 0;

		virtual ~VertexArray() = 0;
	};

	VertexArray::~VertexArray() {}

	//CONSIDER(Ian): This class would work well as a template/macro/code generation thing
	struct VertexColorTextureArray : VertexArray
	{
		VertexColorTexture *Vertices;
		uint32 VertexCount;

		VertexColorTextureArray(VertexColorTexture *vertices, uint32 vertexCount) : Vertices(vertices), VertexCount(vertexCount) {}

		//ASK(Ian):  This modifies the currently set VAO, but does not require that that VAO be passed in.
		//			 How should I best signify the global state that this requires, since modifying the current VAO myself here would cause other issues?
		void BindShaderVariables(GLuint shaderProgram)
		{
			//TODO(Ian): Figure out why this function is causing a GL_INVALID_VALUE error
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

		int32 GetBufferSize()
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

	struct VertexNormalTextureArray : VertexArray
	{
		VertexNormalTexture *Vertices;
		uint32 VertexCount;

		VertexNormalTextureArray(VertexNormalTexture *vertices, uint32 vertexCount) : Vertices(vertices), VertexCount(vertexCount) {}

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

		int32 GetBufferSize()
		{
			return sizeof(VertexNormalTexture) * VertexCount;
		}

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