#ifndef HANDMADE_GEOMETRYHELPERS_H
#define HANDMADE_GEOMETRYHELPERS_H

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
	///Generates a ring of vertices in the given container around the origin of the given transformation.
	///@param transformation The transformation that will be baked into the generated ring.
	///@param color The color that will be given to the vertices.
	///@param count The number of vertices that the ring will have.
	///@param container The structure to add the vertices to.
	void GenerateVertexRing(Matrix4 &transformation, const Vector3 &color, int count, VertexColorTexture *container);

	///Generates a ring of vertices in the given container around the center of the given point around the z axis.
	///@param radius the radius of the vertex ring.
	///@param center The point the vertex ring will be centered on.
	///@param color The color that will be given to the vertices.
	///@param count The number of vertices that the ring will have.
	///@param container The structure to add the vertices to.
	inline void GenerateVertexRing(real32 radius, const Vector3 &center, const Vector3 &color, int count, VertexColorTexture *container)
	{
		Matrix4 transform = Matrix4::CreateScale({ radius, radius, 1 }) * Matrix4::CreateTranslation(center);
		GenerateVertexRing(transform, color, count, container);
	};

	///Creates a 3D arrow mesh.
	///@params color The color of the arrow.
	///@params ringVertices the number of points each cirle will be subdivided into.
	///@params shaderProgram The shader program to draw the arrow with.
	///@params assetName The name the arrow will have in the asset manager.
	///@params assets The asset manager to add this ring to.
	///@params memory The memory arena to create this arrow in.
	Model* MakeArrow(const Vector3 &color, int32 ringVertices, GLuint shaderProgram, HMString assetName, AssetManager& assets, Memory::NewDeleteArena& memory);
}

#endif //HANDMADE_GEOMETRYHELPERS_H