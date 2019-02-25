#ifndef HANDMADE_GEOMETRY_HELPERS_CPP
#define HANDMADE_GEOMETRY_HELPERS_CPP

#include "../handmade_typedefs.h"
#include "../handmade.h"
#include "../content/AssetManager.h"
#include "../content/AssetPtr.h"
#include "../content/MeshCollection.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "../general/StretchyArray.h"
#include "../general/StringHelpers.h"
#include "../general/memory/NewDeleteArena.h"
#include "GeometryHelpers.h"
#include "Texture2D.h"
#include "Material.h"
#include "Model.h"
#include "Vertex.h"
#include "VertexColorTextureArray.h"
#include "defaults/DefaultMaterials.h"
#include "../libraries/glew.h"
#include <cstdlib>

namespace Drawing
{
	void GenerateVertexRing(Matrix4 &transformation, const Vector3 &color, int count, VertexColorTexture *container)
	{
		real32 step = 2.0f * Pi32 / count;
		for (int i = 0; i < count; i++)
		{
			Vector3 position = { cosf(step * i), sinf(step * i), 0 };
			position = transformation.Transform(position);
			VertexColorTexture vertex = { position.x, position.y, position.z, 0, color.g, 1.0f / count * i, 0, 0 };
			container[i] = vertex;
		}
	}

	Model* MakeArrow(const Vector3 &color, int32 ringVertices, GLuint shaderProgram, HMString assetName, AssetManager& assets, Memory::NewDeleteArena& memory)
	{
		/*  Example vertex layout:
		1           0
		4 8 4    1-4 9-16 5-8
		8         17-24
		1           25
		*/
		int32 vertexCount = 2 + ringVertices * 3;
		VertexColorTexture *vertices = new VertexColorTexture[vertexCount];
		VertexColorTexture *currentVertex = vertices;

		*currentVertex = { 0, 0, 1, 1, 1, 1, 0, 0 };
		currentVertex++;
		Vector3 white = { 1, 1, 1 };
		GenerateVertexRing(1, { 0, 0, 0.5f }, white, ringVertices, currentVertex);
		currentVertex += ringVertices;
		GenerateVertexRing(0.5f, { 0, 0, 0.5f }, white, ringVertices, currentVertex);
		currentVertex += ringVertices;
		GenerateVertexRing(0.5f, { 0, 0, 0 }, white, ringVertices, currentVertex);
		currentVertex += ringVertices;
		*currentVertex = { 0, 0, 0, 1, 1, 1, 0, 0 };

		//TODO: Figure out how to consistently decompose a mesh into sensible strips seperated by Primitive Restart Indices, see if that's any faster

		//ringVertices tris on either end, then ringVertices * 2 tris for the two strips of quads in the middle
		uint32 elementCount = ringVertices * 6 * 3;
		GLuint *elements = new GLuint[elementCount];
		uint32 currElement = 0;

		//Build cap
		for (int i = 1; i < ringVertices + 1; i++)
		{
			elements[currElement++] = 0;
			elements[currElement++] = i;
			elements[currElement++] = (i % ringVertices) + 1;
		}

		//Build cap underside
		for (int i = 1; i < ringVertices + 1; i++)
		{
			int32 topRight = (i % ringVertices) + 1;
			int32 bottomLeft = i + ringVertices;
			int32 bottomRight = topRight + ringVertices;

			elements[currElement++] = i;
			elements[currElement++] = bottomLeft;
			elements[currElement++] = topRight;
			elements[currElement++] = bottomRight;
			elements[currElement++] = topRight;
			elements[currElement++] = bottomLeft;
		}

		//Build tube
		for (int i = ringVertices + 1; i < ringVertices * 2 + 1; i++)
		{
			int32 topRight = (i % ringVertices) + ringVertices + 1;
			int32 bottomLeft = i + ringVertices;
			int32 bottomRight = topRight + ringVertices;

			elements[currElement++] = i;
			elements[currElement++] = bottomLeft;
			elements[currElement++] = topRight;
			elements[currElement++] = bottomRight;
			elements[currElement++] = topRight;
			elements[currElement++] = bottomLeft;
		}

		//Build bottom
		for (int i = ringVertices * 2 + 1; i < ringVertices * 3 + 1; i++)
		{
			elements[currElement++] = (i % ringVertices) + ringVertices * 2 + 1;
			elements[currElement++] = i;
			elements[currElement++] = ringVertices * 3 + 1;
		}

		VertexColorTextureArray *verts = new VertexColorTextureArray(vertices, vertexCount);
		//Mesh *arrowMesh = new Mesh(verts, elements, elementCount, shaderProgram, Drawing::Defaults::BlankMaterial);
		Content::MeshCollection arrowGeo = Content::MeshCollection(&memory, 1);
		new (arrowGeo.AddRaw()) Mesh(verts, elements, elementCount, shaderProgram, Drawing::Defaults::BlankMaterial);
		bool success = false;
		Model *arrow = new Model(assets.AddManaged(arrowGeo, assetName, success), { 0, 0, 0 }, { 1, 1, 1 }, Matrix4::Identity(), color);

		return arrow;
	}
}

#endif //HANDMADE_GEOMETRY_HELPERS_CPP