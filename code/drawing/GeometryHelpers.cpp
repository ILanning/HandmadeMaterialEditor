#ifndef HANDMADE_GEOMETRY_HELPERS
#define HANDMADE_GEOMETRY_HELPERS

#include "../handmade_typedefs.h"
#include "../handmade.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "../general/StretchyArray.h"
#include "../general/StringHelpers.h"
#include "Texture2D.h"
#include "Geometry.h"
#include "Model.h"
#include "Vertex.h"
#include "../libraries/glew.h"
#include <cstdlib>

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

void GenerateVertexRing(real32 radius, const Vector3 &center, const Vector3 &color, int count, VertexColorTexture *container)
{
	Matrix4 transform = Matrix4::CreateScale({ radius, radius, 1 }) * Matrix4::CreateTranslation(center);
	GenerateVertexRing(transform, color, count, container);
};

Model* MakeArrow(const Vector3 &color, int32 ringVertices, GLuint shaderProgram)
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

	//TODO(Ian): Figure out how to consistently decompose a mesh into sensible strips seperated by Primitive Restart Indices, see if that's any faster

	//ringVertices tris on either end, then ringVertices * 2 tris for the two strips of quads in the middle
	uint32 elementCount = ringVertices * 6 * 3;
	GLuint *elements = new GLuint[elementCount];
	uint32 currElement = 0;

	//Build cap
	for (int i = 1; i < ringVertices + 1; i++)
	{
		elements[currElement++] = (i % ringVertices) + 1;
		elements[currElement++] = i;
		elements[currElement++] = 0;
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
		elements[currElement++] = bottomLeft;
		elements[currElement++] = topRight;
		elements[currElement++] = bottomRight;
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
		elements[currElement++] = bottomLeft;
		elements[currElement++] = topRight;
		elements[currElement++] = bottomRight;
	}

	//Build bottom
	for (int i = ringVertices * 2 + 1; i < ringVertices * 3 + 1; i++)
	{
		elements[currElement++] = ringVertices * 3 + 1;
		elements[currElement++] = i;
		elements[currElement++] = (i % ringVertices) + ringVertices * 2 + 1;
	}

	//TODO(Ian): Replace this with a reference to a global pure white texture
	Texture2D *texture = new Texture2D(new uint8[4]{ 255, 255, 255, 255 }, 1, 1, GL_RGBA, GL_RGBA);
	VertexColorTextureArray *verts = new VertexColorTextureArray(vertices, vertexCount);
	Geometry *arrowMesh = new Geometry(verts, elements, elementCount, shaderProgram, texture);
	Model *arrow = new Model(arrowMesh, { 0, 0, 0 }, { 1, 1, 1 }, Matrix4::Identity(), color);

	return arrow;
}


Geometry *ParseOBJ(char *file, const int32 fileLength, GLuint shaderProgram, Texture2D *texture)
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

		//TODO(Ian):  This function is not resilient against malformed definitions, find a way to improve that
		static bool TryParse(const char *string, int32 length, ObjVertexNode &outResult, int32 offset = 0, int32 *readFinishIndex = nullptr)
		{
			//Consists of at least two slashes, with some at most three vertex components before, between, and after them
			//Not all spaces need have a value
			//Lists are 1 indexed, leaving space at zero for a default value
			
			ObjVertexNode result;

			int32 readHead = SeekToNonWhitespace(string, length, offset);
			if (readHead == length)
			{
				return false;
			}

			int test = 0;
			if (string[offset + 3] == '3' && string[offset + 4] == '6')
			{
				test++;
			}
			bool readSuccess = TryParseInt32(string, length, result.Position, readHead, &readHead);
			if(!readSuccess && !(string[readHead] == '/' || string[readHead] == '\\'))
			{
				return false;
			}
			readHead++;
			readSuccess = TryParseInt32(string, length, result.Texture, readHead, &readHead);
			if (!readSuccess && !(string[readHead] == '/' || string[readHead] == '\\'))
			{
				return false;
			}
			readHead++;
			if (IsDigit(string[readHead])) //Without this, it could read forward into the next vertex's position part
			{
				TryParseInt32(string, length, result.Normal, readHead, &readHead);
			}

			if (readFinishIndex)
			{
				*readFinishIndex = readHead;
			}
			outResult = result;
			return true;
		}
	};
	
	class ObjVertexBuilder
	{
		StretchyArray<ObjVertexNode> vertexBlueprints;
		StretchyArray<VertexNormalTexture> builtVertices;
		StretchyArray<GLuint> elements;
		StretchyArray<Vector3> positions;
		StretchyArray<Vector3> normals;
		StretchyArray<Vector2> uvs;

		void DeleteNodes(ObjVertexNode head)
		{
			ObjVertexNode *next = head.Next;
			while (next)
			{
				ObjVertexNode *toDelete = next;
				next = next->Next;
				delete toDelete;
			}
		}

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

		ObjVertexBuilder(char *file, int32 fileLength)
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

			bool endOfFile = false;
			int32 nextLineStart = SeekToNonWhitespace(file, fileLength, 0);
			int32 nextLineEnd = GetLineEnd(file, fileLength, nextLineStart);

			while (!endOfFile)
			{
				if (nextLineEnd == fileLength)
				{
					endOfFile = true;
				}
				switch (file[nextLineStart])
				{
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
					int32 faceVerts[4] = {-1, -1, -1, -1};

					int32 offset = nextLineStart + 2;
					for (int32 i = 0; i < 4; i++)
					{
						ObjVertexNode node;
						if (ObjVertexNode::TryParse(file, fileLength, node, offset, &offset))
						{
							int test = 0;
							if(node.Position == 0)
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
						elements.PushBack((GLuint)faceVerts[1]);
						elements.PushBack((GLuint)faceVerts[3]);
					}

					break;
				}
				case 's': //Shading Group
				{
					//TODO(Ian): Recognize shading group line
					break;
				}
				case 'g': //Material Group
				{
					//TODO(Ian): Recognize material group line
					break;
				}
				case 'm': //Material file
				{
					//TODO(Ian): Recognize mtlfile line
					break;
				}
				case 'u': //usemtl
				{

				}
				default:
					//TODO(Ian):  Warn/fail over non-polygonal geometry data, since we don't care about supporting that
					break;
				}

				nextLineStart = SeekToNonWhitespace(file, fileLength, nextLineEnd);
				nextLineEnd = GetLineEnd(file, fileLength, nextLineStart);
			}
		}

		Geometry *ExportGeometry(GLuint shaderProgram, Texture2D *texture)
		{
			VertexNormalTextureArray *vertexArray = new VertexNormalTextureArray(builtVertices.ToArray(), builtVertices.Length());
			GLuint *elementArray = elements.ToArray();
			uint32 elementCount = elements.Length();

			Geometry *result = new Geometry(vertexArray, elementArray, elementCount, shaderProgram, texture);

			return result;
		}

		~ObjVertexBuilder()
		{
			int32 arraySize = vertexBlueprints.Length();
			for (int i = 0; i < arraySize; i++)
			{
				DeleteNodes(vertexBlueprints[i]);
			}
		}
	};

	ObjVertexBuilder builder = ObjVertexBuilder(file, fileLength);

	//TODO(Ian): Needs a shader program, possibly a different one for each material type.  Figure out how to deal with that
	//Maybe a general shader manager with a library of shader programs to pull from?
	//TODO(Ian): Creates a Texture2D resource and does not manage it, handle that once there's a ContentManager

	

	//For each smoothing group, maintain an unordered list of the vector indices that have already been added
	//Since our data is a contiguous list, we can just use a bool array instead of an unordered list
	return builder.ExportGeometry(shaderProgram, texture);
}

#endif