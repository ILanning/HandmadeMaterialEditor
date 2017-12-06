#ifndef HANDMADE_OBJ_LOADER
#define HANDMADE_OBJ_LOADER

#include "../handmade_typedefs.h"
#include "../handmade.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../general/StretchyArray.h"
#include "../general/StringHelpers.cpp"
#include "../general/PathHelpers.cpp"
#include "../drawing/Texture2D.h"
#include "../drawing/Mesh.h"
#include "../drawing/Geometry.h"
#include "../drawing/Vertex.h"
#include "../drawing/defaults/DefaultMaterials.h"
#include "../libraries/glew.h"
#include "MTLLoader.h"
#include <cstdlib>

namespace Content
{
	namespace OBJ
	{
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

			//TODO(Ian):  This function is not very resilient against malformed definitions, find a way to improve that
			static bool TryParse(const char *string, int32 length, ObjVertexNode &outResult, int32 offset = 0, int32 *readFinishIndex = nullptr)
			{
				//Consists of at least two slashes, with some at most three vertex components before, between, and after them
				//Not all spaces need have a value
				//Lists are 1 indexed, leaving space at zero for a default value
				
				ObjVertexNode result;

				int32 readHead = CString::FindNonWhitespace(string, length, offset);
				if (readHead == length)
				{
					return false;
				}

				int test = 0;
				if (string[offset + 3] == '3' && string[offset + 4] == '6')
				{
					test++;
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

			void Reset()
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
		};

		class ObjParser
		{
			StretchyArray<Mesh *> meshes;
			StretchyArray<Material> materials;
			StretchyArray<Texture2D *> textures;

			StretchyArray<ObjVertexNode> vertexBlueprints;
			StretchyArray<VertexNormalTexture> builtVertices;
			StretchyArray<GLuint> elements;
			StretchyArray<Vector3> positions;
			StretchyArray<Vector3> normals;
			StretchyArray<Vector2> uvs;

		public:

			int32 FindOrCreate(const ObjVertexNode &node)
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

			void PushMesh(GLuint shaderProgram, Material *mat)
			{
				VertexNormalTextureArray *vertexArray = new VertexNormalTextureArray(builtVertices.ToArray(), builtVertices.Length());
				GLuint *elementArray = elements.ToArray();
				uint32 elementCount = elements.Length();

				Mesh *mesh = new Mesh(vertexArray, elementArray, elementCount, shaderProgram, new Material(*mat));
				meshes.PushBack(mesh);

				//Clear builtVertices, elements, and possibly vertexBlueprints as well
				builtVertices = StretchyArray<VertexNormalTexture>();
				elements = StretchyArray<GLuint>();
				int32 count = vertexBlueprints.Length();
				for (int32 i = 0; i < count; i++)
				{
					vertexBlueprints[i].Reset();
				}
			}

			ObjParser(FileData toLoad, ReadFileFunc *readFile, GLuint shaderProgram)
			{
				builtVertices = StretchyArray<VertexNormalTexture>();
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

				materials = StretchyArray<Material>();
				meshes = StretchyArray<Mesh*>();
				textures = StretchyArray<Texture2D*>();
				Material *setMaterial = Drawing::Defaults::BlankMaterial;

				char *file = (char *)toLoad.File;
				int32 fileLength = toLoad.FileSize;

				bool endOfFile = false;
				int32 nextLineStart = CString::FindNonWhitespace(file, fileLength, 0);
				int32 nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);

				while (!endOfFile)
				{
					if (nextLineEnd == fileLength)
					{
						endOfFile = true;
					}
					switch (file[nextLineStart])
					{
						//TODO(Ian): Examine lod support
					case 'v': //Vector components
					{
						switch (file[nextLineStart + 1])
						{
						case ' ': //Position component
						{
							//TODO(Ian): A vertex color can be embedded in these lines, handle that
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
						//Add Id to element array
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
							elements.PushBack((GLuint)faceVerts[0]);
							elements.PushBack((GLuint)faceVerts[2]);
							elements.PushBack((GLuint)faceVerts[1]);
						}
						else
						{
							elements.PushBack((GLuint)faceVerts[0]);
							elements.PushBack((GLuint)faceVerts[2]);
							elements.PushBack((GLuint)faceVerts[1]);

							elements.PushBack((GLuint)faceVerts[2]);
							elements.PushBack((GLuint)faceVerts[0]);
							elements.PushBack((GLuint)faceVerts[3]);
						}

						break;
					}
					case 's': //Shading Group
					{
						//TODO(Ian): Recognize shading group line
						break;
					}
					case 'g': //Group
					{
						//TODO(Ian): Recognize group line
						break;
					}
					case 'm': //Material file
					{
						int32 offset = nextLineStart + 7;

						int32 pathLength = 0;
						char *path = CString::CopySubstring(file, nextLineEnd - offset, &pathLength, fileLength, offset);
						if (Path::IsRelative(path))
						{
							char *pathStart = Path::GetParentDirectory(toLoad.Path);
							char *pathEnd = path;

							path = Path::Combine(pathStart, pathEnd, 0, 0, &pathLength);

							delete[] pathStart;
							delete[] pathEnd;
						}

						ParseMTL(path, pathLength, readFile, &materials);
						delete[] path;

						break;
					}
					case 'u': //usemtl
					{
						int32 materialNameStart = CString::FindNonWhitespace(file, fileLength, nextLineStart + 6);						

						//Search through materials by name
						bool foundMaterial = false;
						Material *currMat = nullptr;

						int32 matCount = materials.Length();
						for (int32 i = 0; i < matCount; i++)
						{
							currMat = &(materials[i]);
							if (CString::IsEqual(file, currMat->Name, nextLineEnd, currMat->NameLength - 1, materialNameStart, 0))
							{
								foundMaterial = true;
								break;
							}
						}
						if (foundMaterial)
						{
							//If the current list of built vertices and elements has any values, package that into a new finished mesh
							if (builtVertices.Length() > 0)
							{
								PushMesh(shaderProgram, currMat);
							}

							setMaterial = currMat;
						}
					}
					default:
						//TODO(Ian):  Warn over non-polygonal geometry data, since we don't care about supporting that
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

			Geometry *ExportGeometry()
			{
				//VertexNormalTextureArray *vertexArray = new VertexNormalTextureArray(builtVertices.ToArray(), builtVertices.Length());
				//GLuint *elementArray = elements.ToArray();
				//uint32 elementCount = elements.Length();

				//Mesh *mesh = new Mesh(vertexArray, elementArray, elementCount, shaderProgram, new Material(materials[0]));
				int32 arraySize = meshes.Length();
				Mesh *meshArray = new Mesh[arraySize];
				for (int32 i = 0; i < arraySize; i++)
				{
					swap(meshArray[i], *meshes[i]);
					delete meshes[i];
				}

				Geometry *result = new Geometry(meshArray, arraySize);

				return result;
			}

			Material *ExportMaterials(int32 &outCount)
			{
				outCount = materials.Length();
				return materials.ToArray();
			}

			~ObjParser()
			{
				int32 arraySize = vertexBlueprints.Length();
				for (int i = 0; i < arraySize; i++)
				{
					vertexBlueprints[i].Reset();
				}
			}
		};
	}

	Geometry *ParseOBJ(char *path, const int32 pathLength, GLuint shaderProgram, ReadFileFunc *readFile)
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

		bool success = false;
		FileData file = readFile(path, pathLength, &success);
		if (success)
		{
			OBJ::ObjParser builder = OBJ::ObjParser(file, readFile, shaderProgram);
			return builder.ExportGeometry();
		}
		else
		{
			//TODO(Ian): Logging, provide a way to report a failure to load the file
			return nullptr;
		}

		//TODO(Ian): Needs a shader program, possibly a different one for each material type.  Figure out how to deal with that
		//Maybe a general shader manager with a library of shader programs to pull from?
		//TODO(Ian): Creates a Texture2D resource and does not manage it, handle that once there's a ContentManager



		//For each smoothing group, maintain an unordered list of the vector indices that have already been added
		//Since our data is a contiguous list, we can just use a bool array instead of an unordered list
		
	}
}

#endif