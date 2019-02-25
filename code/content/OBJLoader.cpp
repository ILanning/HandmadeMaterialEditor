#ifndef HANDMADE_OBJLOADER_CPP
#define HANDMADE_OBJLOADER_CPP

#include "OBJLoader.h"
#include "../general/PathHelpers.h"
#include "../general/StringHelpers.h"
#include "../general/StaticArray.h"
#include "../drawing/VertexNormalTextureArray.h"
#include "../drawing/defaults/DefaultMaterials.h"
#include "MTLLoader.h"
#include <cstdlib>

namespace Content
{
	namespace OBJ
	{
		using namespace Drawing;

		//TODO:  This function is not very resilient against malformed definitions, find a way to improve that
		///Attempts to parse a single vertex line from an OBJ file definition.
		bool ObjParser::ObjVertexNode::TryParse(const char *string, int32 length, ObjVertexNode &outResult, int32 offset, int32 *readFinishIndex = nullptr)
		{
			//Consists of at least two slashes, with some at most three vertex components before, between, and after them
			//Not all spaces need have a value
			//Lists are 1 indexed, leaving space at zero for a default value

			ObjVertexNode result;

			int32 readHead = CString::FindNonWhitespace(string, length, offset);
			if (readHead == length || readHead == -1)
			{
				return false;
			}

			bool readSuccess = CString::TryParseInt32(string, length, result.Position, readHead, &readHead);
			if (!readSuccess && !(string[readHead] == '/' || string[readHead] == '\\'))
			{
				return false;
			}
			readHead++;
			readSuccess = CString::TryParseInt32(string, length, result.Texture, readHead, &readHead);
			if (!readSuccess && !(string[readHead] == '/' || string[readHead] == '\\'))
			{
				return false;
			}
			readHead++;
			if (CString::IsDigit(string[readHead])) //Without this, it could read forward into the next vertex's position part
			{
				CString::TryParseInt32(string, length, result.Normal, readHead, &readHead);
			}

			if (readFinishIndex)
			{
				*readFinishIndex = readHead;
			}
			outResult = result;
			return true;
		}

		void ObjParser::ObjVertexNode::ResetHead()
		{
			ObjVertexNode *next = this->Next;
			while (next)
			{
				ObjVertexNode *toDelete = next;
				next = next->Next;
				delete toDelete;
			}

			this->FinalIndex = -1;
			this->Normal = -1;
			this->Texture = -1;
			this->Next = nullptr;
		}

		ObjParser::ObjParser(FileData toLoad, ReadFileFunc *readFile, GLuint shaderProgram, AssetManager& assets, Memory::NewDeleteArena* memory)
		{
			builtVertices = StretchyArray<Drawing::VertexNormalTexture>();
			elements = StretchyArray<GLuint>();

			//Lists start at 1 in the file, so 0 is filled with a default value
			//A final vertex definition *does not* have to provide info for all values,
			//so the 0 values will be used if nothing else is given.
			vertexBlueprints = StretchyArray<ObjVertexNode>();
			vertexBlueprints.PushBack(ObjVertexNode(-1, -1, -1));
			positions = StretchyArray<Vector3>();
			positions.PushBack({ 0, 0, 0 });
			normals = StretchyArray<Vector3>();
			normals.PushBack({ 0, 0, 0 });
			uvs = StretchyArray<Vector2>();
			uvs.PushBack({ 0, 0 });

			materials = Collections::HashMap<HMString, AssetPtr<Material>, Memory::NewDeleteArena>(memory);
			meshes = Collections::ArrayList<Mesh, Memory::NewDeleteArena>();
			AssetPtr<Material> setMaterial = Drawing::Defaults::BlankMaterial;

			char *file = (char *)toLoad.File;
			int32 fileLength = toLoad.FileSize;

			bool endOfFile = false;
			int32 nextLineStart = CString::FindNonWhitespace(file, fileLength);
			int32 nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);

			while (!endOfFile)
			{
				if (nextLineEnd == fileLength)
				{
					endOfFile = true;
				}
				switch (file[nextLineStart])
				{
					//TODO: Examine lod support
				case 'v': //Vector components
				{
					switch (file[nextLineStart + 1])
					{
					case ' ': //Position component
					{
						//TODO: A vertex color can be embedded in these lines, handle that
						//Create an array for 6 floats, parse the line until you hit a newline, then see how many floats you got
						//Can a vertex color have alpha?
						//.obj file vector line layout:
						//v [x component] [y component] [z component]
						//ex. v 2.345354 7.431235 8.324325

						Vector3 result = {};
						char *nextFloat = file + nextLineStart + 2;

						for (int32 i = 0; i < Vector3::ElementCount; i++)
						{
							result.elements[i] = strtof(nextFloat, &nextFloat);
						}

						positions.PushBack(result);
						vertexBlueprints.PushBack(ObjVertexNode());
						break;
					}
					case 'n': //Normal component
					{
						Vector3 result = {};
						char *nextFloat = file + nextLineStart + 2;

						for (int32 i = 0; i < Vector3::ElementCount; i++)
						{
							result.elements[i] = strtof(nextFloat, &nextFloat);
						}
						normals.PushBack(result);
						break;
					}
					case 't': //Texture/UV component
					{
						//.obj file vector line layout:
						//v [x component] [y component] [z component]
						//ex. v 2.345354 7.431235 8.324325

						Vector2 result = {};
						char *nextFloat = file + nextLineStart + 2;

						for (int32 i = 0; i < Vector2::ElementCount; i++)
						{
							result.elements[i] = strtof(nextFloat, &nextFloat);
						}

						uvs.PushBack(result);
						break;
					}
					}
					break;
				}
				case 'f': //Face
				{
					//Take in vertex group

					//Make a vertex from it and return the ID
					//Add ID to element array
					//If there are four vertex groups in the definition, repeat the middle two
					int32 faceVerts[4] = { -1, -1, -1, -1 };

					int32 offset = nextLineStart + 2;
					for (int32 i = 0; i < 4; i++)
					{
						ObjVertexNode node;
						if (ObjVertexNode::TryParse(file, fileLength, node, offset, &offset))
						{
							int test = 0;
							if (node.Position == 0)
							{
								test++;
							}
							faceVerts[i] = FindOrCreate(node);
						}
					}
					if (faceVerts[3] == -1)
					{
						elements.PushBack((GLuint)faceVerts[1]);
						elements.PushBack((GLuint)faceVerts[2]);
						elements.PushBack((GLuint)faceVerts[0]);
					}
					else
					{
						elements.PushBack((GLuint)faceVerts[1]);
						elements.PushBack((GLuint)faceVerts[2]);
						elements.PushBack((GLuint)faceVerts[0]);

						elements.PushBack((GLuint)faceVerts[3]);
						elements.PushBack((GLuint)faceVerts[0]);
						elements.PushBack((GLuint)faceVerts[2]);
					}

					break;
				}
				case 's': //Shading Group
				{
					//TODO: Recognize shading group line
					break;
				}
				case 'g': //Group
				{
					//TODO: Recognize group line
					break;
				}
				case 'm': //Material file
				{
					int32 offset = nextLineStart + 7;

					int32 pathLength = 0;
					char *path = CString::CopySubstring(file + offset, nextLineEnd - offset, &pathLength, fileLength - offset);
					if (Path::IsRelative(path))
					{
						char *pathStart = Path::CloneParentDirectory(toLoad.Path);
						char *pathEnd = path;

						path = Path::Combine(pathStart, pathEnd, 0, 0, &pathLength);

						delete[] pathStart;
						delete[] pathEnd;
					}
					auto results = ParseMTL({ path, (uint32)pathLength }, readFile, *memory, assets);
					materials.AddMany(results);
					memory->Deallocate(results.Data);

					delete[] path;
					break;
				}
				case 'u': //usemtl
				{
					int32 materialNameStart = CString::FindNonWhitespace(file, fileLength, nextLineStart + 6);

					int32 useNameLength = 0;
					char* useNameChars = CString::CopySubstring(file + materialNameStart, nextLineEnd - materialNameStart, &useNameLength, nextLineEnd);
					HMString useName = { useNameChars, (uint32)useNameLength };
					
					if (materials.CheckExists(useName))
					{
						AssetPtr<Material> currMat = materials[useName];
						//If the current list of built vertices and elements has any values, package that into a new finished mesh
						if (builtVertices.Length() > 0)
						{
							PushMesh(shaderProgram, currMat);
						}

						setMaterial = currMat;
					}

					delete[] useNameChars;
				}
				default:
					//TODO:  Warn over non-polygonal geometry data, since we don't care about supporting that
					break;
				}

				nextLineStart = CString::FindNonWhitespace(file, fileLength, nextLineEnd);
				if (nextLineStart == -1)
				{
					break;
				}
				nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);
			}

			if (builtVertices.Length() > 0)
			{
				PushMesh(shaderProgram, setMaterial);
			}
		}

		int32 ObjParser::FindOrCreate(const ObjVertexNode &node)
		{
			if (vertexBlueprints[node.Position].FinalIndex != -1)
			{
				if (vertexBlueprints[node.Position].CompareValues(node))
				{
					return vertexBlueprints[node.Position].FinalIndex;
				}
				else
				{
					ObjVertexNode *curr = &vertexBlueprints[node.Position];
					while (curr->Next)
					{
						curr = curr->Next;
						if (curr->CompareValues(node))
						{
							return curr->FinalIndex;
						}
					}
					ObjVertexNode newNode = { node.Position, node.Normal, node.Texture };
					Vector3 pos = positions[node.Position];
					Vector3 norm = normals[node.Normal];
					Vector2 tex = uvs[node.Texture];
					newNode.FinalIndex = builtVertices.PushBack({ pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tex.u, tex.v });

					curr->Next = new ObjVertexNode(newNode);
					return newNode.FinalIndex;
				}
			}
			else
			{
				ObjVertexNode newNode = ObjVertexNode(node.Position, node.Normal, node.Texture);
				Vector3 pos = positions[node.Position];
				Vector3 norm = normals[node.Normal];
				Vector2 tex = uvs[node.Texture];
				newNode.FinalIndex = builtVertices.PushBack({ pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tex.u, tex.v });
				vertexBlueprints[node.Position] = newNode;
				return newNode.FinalIndex;
			}
		}

		void ObjParser::PushMesh(GLuint shaderProgram, AssetPtr<Material> mat)
		{
			VertexNormalTextureArray *vertexArray = new VertexNormalTextureArray(builtVertices.ToArray(), builtVertices.Length());
			GLuint *elementArray = elements.ToArray();
			uint32 elementCount = elements.Length();

			new (meshes.AddRaw()) Mesh(vertexArray, elementArray, elementCount, shaderProgram, mat);

			//Clear builtVertices, elements, and possibly vertexBlueprints as well
			builtVertices = StretchyArray<VertexNormalTexture>();
			elements = StretchyArray<GLuint>();
			int32 count = vertexBlueprints.Length();
			for (int32 i = 0; i < count; i++)
			{
				vertexBlueprints[i].ResetHead();
			}
		}

		MeshCollection ObjParser::ExportGeometry()
		{
			return meshes;
		}

		Collections::HashMap<HMString, AssetPtr<Drawing::Material>, Memory::NewDeleteArena> ObjParser::ExportMaterials()
		{
			return materials;
		}
	}

	MeshCollection ParseOBJ(char *path, int32 pathLength, GLuint shaderProgram, 
		ReadFileFunc *readFile, AssetManager& assets, Memory::NewDeleteArena* memory)
	{
		bool success = false;
		FileData file = readFile(path, pathLength, &success);
		if (success)
		{
			return OBJ::ObjParser(file, readFile, shaderProgram, assets, memory).ExportGeometry();
		}
		else
		{
			//TODO: Logging, provide a way to report a failure to load the file
			return MeshCollection();
		}
	}
}

#endif // !HANDMADE_OBJLOADER_CPP
