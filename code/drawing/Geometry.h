#ifndef HANDMADE_GEOMETRY
#define HANDMADE_GEOMETRY

#include "../handmade_typedefs.h"
#include "../libraries/glew.h"
#include "Mesh.h"

namespace Drawing
{
	struct Geometry
	{
		//CONSIDER(Ian): Maybe this should be an array of pointers
		Mesh *Meshes = nullptr;
		int32 MeshCount = 0;

		Geometry(Mesh *meshes, int32 meshCount) : Meshes(meshes), MeshCount(meshCount)
		{

		}

		void Draw(const Matrix4 &mvp, const Vector3 &color)
		{
			for (int32 i = 0; i < MeshCount; i++)
			{
				Meshes[i].Draw(mvp, color);
			}
		}

		~Geometry()
		{
			delete[] Meshes;
		}
	};
}

#endif