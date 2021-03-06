#ifndef HANDMADE_OBJLOADER_H
#define HANDMADE_OBJLOADER_H

#include "../handmade.h"
#include "../handmade_typedefs.h"
#include "AssetManager.h"
#include "AssetPtr.h"
#include "../content/MeshCollection.h"
#include "../drawing/Mesh.h"
#include "../drawing/Texture2D.h"
#include "../drawing/Vertex.h"
#include "../general/ArrayList.h"
#include "../general/HashMap.h"
#include "../general/HMString.h"
#include "../general/StretchyArray.h"
#include "../general/memory/NewDeleteArena.h"
#include "../libraries/glew.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"

namespace Content
{
	namespace OBJ
	{
		using namespace Drawing;

		class ObjParser
		{
			/*
			Verts/UVs/Normals are referred to by index *from the beginning of the file*.
			Consider: Indexed list of all verts, where each item is actually a pointer to a list
			The list owns a vertex position, and also a set of vertex/texture/normal index sets
			When first loading the file, load all vertex positions into this list, indexed in the same way as in the file
			Simultaneously create more regular lists for the UVs and normals, also indexed as in the file

			Seek through file once, count number of verts, UVs, normals, and elements
			VertCount: Number of lines starting with "v "
			UVCount: Number of lines starting with "vt "
			NormalCount: Number of lines starting with "vn "
			ElementCount: Number of lines starting with "f " times three, plus one for lines containing 4 groups if drawing as GL_TRIANGLE_STRIP or plus three for GL_TRIANGLE

			Seek through file again, actually filling out array
			Vertex/UV/Normal: Call ParseOBJ[x]Line on the string
			Elements: Build a final vertex for the defined element if none exists, then call
			*/

			//For each smoothing group, maintain an unordered list of the vector indices that have already been added
			//Since our data is a contiguous list, we can just use a bool array instead of an unordered list


			struct ObjVertexNode
			{
				int32 Position = 0;
				int32 Normal = 0;
				int32 Texture = 0;
				int32 FinalIndex = -1;
				ObjVertexNode *Next = nullptr;

				ObjVertexNode() {}

				ObjVertexNode(int32 positionIndex, int32 normalIndex, int32 uvIndex) : Position(positionIndex), Normal(normalIndex), Texture(uvIndex) { }

				bool CompareValues(const ObjVertexNode &b)
				{
					return this->Position == b.Position && this->Normal == b.Normal && this->Texture == b.Texture;
				}

				bool CompareValues(int32 positionIndex, int32 normalIndex, int32 textureIndex)
				{
					return this->Position == positionIndex && this->Normal == normalIndex && this->Texture == textureIndex;
				}

				static bool TryParse(const char *string, int32 length, ObjVertexNode &outResult, int32 offset, int32 *readFinishIndex);

				void ResetHead();
			};


			MeshCollection meshes;
			Collections::HashMap<HMString, AssetPtr<Drawing::Material>, Memory::NewDeleteArena> materials;

			StretchyArray<ObjVertexNode> vertexBlueprints;
			StretchyArray<Drawing::VertexNormalTexture> builtVertices;
			StretchyArray<GLuint> elements;
			StretchyArray<Vector3> positions;
			StretchyArray<Vector3> normals;
			StretchyArray<Vector2> uvs;

			Memory::NewDeleteArena* memory;

		public:

			int32 FindOrCreate(const ObjVertexNode &node);

			void PushMesh(GLuint shaderProgram, AssetPtr<Drawing::Material> mat);

			ObjParser(FileData toLoad, ReadFileFunc *readFile, GLuint shaderProgram, AssetManager& assets, Memory::NewDeleteArena* memory);

			Content::MeshCollection ExportGeometry();

			Collections::HashMap<HMString, AssetPtr<Drawing::Material>, Memory::NewDeleteArena> ExportMaterials();

			~ObjParser()
			{
				int32 arraySize = vertexBlueprints.Length();
				for (int i = 0; i < arraySize; i++)
				{
					vertexBlueprints[i].ResetHead();
				}
			}
		};
	}

	MeshCollection ParseOBJ(char *path, int32 pathLength, GLuint shaderProgram,
		ReadFileFunc *readFile, AssetManager& assets, Memory::NewDeleteArena* memory);
}

#endif //HANDMADE_OBJLOADER_H