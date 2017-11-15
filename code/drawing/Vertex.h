#ifndef HANDMADE_VERTEX
#define HANDMADE_VERTEX

#include "../handmade_typedefs.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../libraries/glew.h"

//TODO(Ian):  Define a VertexInfo class with one child class for each type of vertex, to 
//            better enable functions that can handle many types of vertices

struct VertexColorTexture
{
	static const int32 VertexSize = 8;
	union
	{
		GLfloat elements[VertexSize];
		struct
		{
			Vector3 position;
			Vector3 color;
			Vector2 texture;
		};
	};
};

struct VertexNormalTexture
{
	static const int32 VertexSize = 8;
	union
	{
		GLfloat elements[VertexSize];
		struct
		{
			Vector3 position;
			Vector3 normal;
			Vector2 texture;
		};
	};
};

struct VertexArray
{
	virtual void BindShaderVariables(GLuint shaderProgram) = 0;
	virtual int32 GetBufferSize() = 0;
	virtual GLfloat *GetBuffer() = 0;

	virtual ~VertexArray() = 0;
};

VertexArray::~VertexArray() {}

struct VertexColorTextureArray : VertexArray
{
	VertexColorTexture *Vertices;
	uint32 VertexCount;

	VertexColorTextureArray(VertexColorTexture *vertices, uint32 vertexCount) : Vertices(vertices), VertexCount(vertexCount) {}

	//ASK(Ian):  This modifies the currently set VAO, but does not require that that VAO be passed in.
	//			 How should I best signify the global state that this requires, since modifying the current VAO myself here would cause other issues?
	void BindShaderVariables(GLuint shaderProgram)
	{
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

#endif