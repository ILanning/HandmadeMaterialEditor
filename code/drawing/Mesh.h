#ifndef HANDMADE_MESH
#define HANDMADE_MESH

#include "../handmade_typedefs.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "Texture2D.h"
#include "Vertex.h"
#include "Material.h"
#include "../libraries/glew.h"
#include "../general/DebugHelpers.cpp"

struct Mesh
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

	Material *MeshMaterial;

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

	Mesh(VertexArray *vertices, GLuint *elements, uint32 elementCount, GLuint shaderProgram,
		Material *material = nullptr, bool genBuffers = true, GLenum mode = GL_TRIANGLES, uint32 primitiveRestartIndex = MaxInt32)
		: Vertices(vertices), Elements(elements), ElementCount(elementCount), MeshMaterial(material), Mode(mode), PrimitiveRestartIndex(primitiveRestartIndex)
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
		if (MeshMaterial)
		{
			MeshMaterial->Use();
		}

		glDrawElements(Mode, (GLsizei)ElementCount, GL_UNSIGNED_INT, 0);
	}


	~Mesh()
	{
		delete[] Vertices;
		delete[] Elements;

		GLuint buffers[] = { VBO, EBO };
		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &VAO);
		//CONSIDER(Ian):  Eventually a ContentManager will be handling assets, including textures. As a result, we probably don't want to delete the textures this holds.
		//Should there be something here that tells the ContentManager that the textures held by this object have one less dependency?
	}
};

#endif