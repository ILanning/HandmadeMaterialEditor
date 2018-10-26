#ifndef HANDMADE_VERTEX_H
#define HANDMADE_VERTEX_H

#include "../handmade_typedefs.h"
#include "../math/Vector2.cpp"
#include "../math/Vector3.cpp"
#include "../libraries/glew.h"

//TODO(Ian):  Define a VertexInfo class with one child class for each type of vertex, to 
//            better enable functions that can handle many types of vertices
namespace Drawing
{
	struct VertexColorTexture
	{
		static const int32 VertexSize = 8;
		union
		{
			GLfloat elements[VertexSize];
			struct
			{
				Vector3 position;
				Vector3 color;
				Vector2 texture;
			};
		};
	};

	struct VertexNormalTexture
	{
		static const int32 VertexSize = 8;
		union
		{
			GLfloat elements[VertexSize];
			struct
			{
				Vector3 position;
				Vector3 normal;
				Vector2 texture;
			};
		};
	};
}
#endif //HANDMADE_VERTEX_H