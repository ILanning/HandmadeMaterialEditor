#ifndef HANDMADE_GEOMETRY
#define HANDMADE_GEOMETRY

#include "../handmade_typedefs.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "Texture2D.h"
#include "Vertex.h"
#include "../libraries/glew.h"

struct Geometry
{
	VertexArray *Vertices;
	GLuint *Elements;
	uint32 ElementCount;
	GLenum Mode;
	uint32 PrimitiveRestartIndex;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLint MVPUniform;
	GLint ColorUniform;

	Texture2D *Texture;

	bool Initialized;

	void CreateBufferObjects(GLuint shaderProgram)
	{
		//NOTE(Ian): Ordering here is important, these are commands to the OpenGL state machine. glBufferData operates on whatever buffer was most recently bound.
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices->GetBufferSize(), Vertices->GetBuffer(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ElementCount, Elements, GL_STATIC_DRAW);

		glUseProgram(shaderProgram);

		Vertices->BindShaderVariables(shaderProgram);

		ColorUniform = glGetUniformLocation(shaderProgram, "meshColor");
		MVPUniform = glGetUniformLocation(shaderProgram, "mvp");
	}

	void UpdateBufferObjects()
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices->GetBufferSize(), Vertices->GetBuffer(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ElementCount, Elements, GL_STATIC_DRAW);
	}

	Geometry(VertexArray *vertices, GLuint *elements, uint32 elementCount, GLuint shaderProgram,
		Texture2D *texture = NULL, bool genBuffers = true, GLenum mode = GL_TRIANGLES, uint32 primitiveRestartIndex = 0xffffffff)
		: Vertices(vertices), Elements(elements), ElementCount(elementCount), Texture(texture), Mode(mode), PrimitiveRestartIndex(primitiveRestartIndex)
	{
		if (genBuffers)
		{
			CreateBufferObjects(shaderProgram);
			Initialized = true;
		}
		else
		{
			VAO = 0;
			VBO = 0;
			EBO = 0;
			MVPUniform = -1;
			ColorUniform = -1;
			Initialized = false;
		}
	}

	void Draw(const Matrix4 &mvp, const Vector3 &color)
	{
		glBindVertexArray(VAO);
		glUniform3fv(ColorUniform, 1, color.elements);
		glUniformMatrix4fv(MVPUniform, 1, GL_TRUE, mvp.elements);
		if (Texture)
		{
			Texture->Bind();
		}

		glDrawElements(Mode, (GLsizei)ElementCount, GL_UNSIGNED_INT, 0);
	}

	//TODO(Ian): Write Geometry destructor
};

#endif