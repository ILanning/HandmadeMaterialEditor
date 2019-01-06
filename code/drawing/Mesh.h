#ifndef HANDMADE_MESH_H
#define HANDMADE_MESH_H

#include "../handmade_typedefs.h"
#include "../content/AssetPtr.h"
#include "../math/Vector3.cpp"
#include "../math/Matrix4.h"
#include "Texture2D.h"
#include "VertexArray.h"
#include "Material.h"
#include "../libraries/glew.h"
#include "../general/DebugHelpers.cpp"

namespace Drawing
{
	struct Mesh
	{
		VertexArray *Vertices = nullptr;
		GLuint *Elements = nullptr;
		uint32 ElementCount = 0;
		GLenum Mode = GL_TRIANGLES;
		uint32 PrimitiveRestartIndex = MaxInt32;

		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;

		GLint MVPUniform = -1;
		GLint ColorUniform = -1;

		AssetPtr<Material> MeshMaterial = {};

		bool Initialized = false;

		void CreateBufferObjects(GLuint shaderProgram)
		{
			//NOTE: Ordering here is important, these are commands to the OpenGL state machine. glBufferData operates on whatever buffer was most recently bound.
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

		Mesh() {}

		Mesh(VertexArray *vertices, GLuint *elements, uint32 elementCount, GLuint shaderProgram,
			AssetPtr<Material> material = {}, bool genBuffers = true, GLenum mode = GL_TRIANGLES, uint32 primitiveRestartIndex = MaxInt32)
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

		void Draw(const Matrix4 &mvp, const Vector3 &color) const
		{
			glBindVertexArray(VAO);
			glUniform3fv(ColorUniform, 1, color.elements);
			glUniformMatrix4fv(MVPUniform, 1, GL_TRUE, mvp.elements);
			if (!MeshMaterial.IsNull())
			{
				MeshMaterial->Use();
			}

			glDrawElements(Mode, (GLsizei)ElementCount, GL_UNSIGNED_INT, 0);
		}

		friend void swap(Mesh &first, Mesh &second)
		{
			uint8 temp[sizeof(Mesh)];
			memcpy(temp, &first, sizeof(Mesh));
			memcpy(&first, &second, sizeof(Mesh));
			memcpy(&second, temp, sizeof(Mesh));
		}

		Mesh(Mesh& other)
		{
			swap(*this, other);
		}

		Mesh(Mesh &&other)
		{
			swap(*this, other);
		}

		Mesh &operator=(Mesh other)
		{
			swap(*this, other);
			return *this;
		}

		Mesh &operator=(Mesh&& other)
		{
			swap(*this, other);
			return *this;
		}

		~Mesh()
		{
			if (Vertices && Elements)
			{
				GLuint buffers[] = { VBO, EBO };
				glDeleteBuffers(2, buffers);
				glDeleteVertexArrays(1, &VAO);
			}
			delete[] Vertices;
			delete[] Elements;
		}

		/*void swap(HashMap& other)
		{
			real32 tempFloat = other.upsizeFactor;
			other.upsizeFactor = upsizeFactor;
			upsizeFactor = tempFloat;

			Allocator* tempMem = other.memory;
			other.memory = memory;
			memory = tempMem;

			HashItem* tempItems = other.buckets;
			other.buckets = buckets;
			buckets = tempItems;

			int32 tempInt = other.bucketCount;
			other.bucketCount = bucketCount;
			bucketCount = tempInt;

			tempInt = other.occupied;
			other.occupied = occupied;
			occupied = tempInt;

			tempInt = other.maxAlreadyFilled;
			other.maxAlreadyFilled = maxAlreadyFilled;
			maxAlreadyFilled = tempInt;
		}

		HashMap(HashMap& other)
		{
			memory = other.memory;
			bucketCount = other.bucketCount;
			occupied = other.occupied;
			buckets = memory->Allocate<HashItem>(bucketCount);

			for (int32 i = 0; i < bucketCount; i++)
			{
				buckets[i] = other.buckets[i];
			}
		}

		HashMap(HashMap&& other) : HashMap()
		{
			swap(other);
		}

		HashMap& operator=(HashMap other)
		{
			swap(other);
			return *this;
		}

		~HashMap()
		{
			if (memory && buckets)
			{
				memory->Deallocate(buckets);
				buckets = nullptr;
				memory = nullptr;
			}

			bucketCount = 0;
			occupied = 0;
		}*/
	};
}

#endif //HANDMADE_MESH_H