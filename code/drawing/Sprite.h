#ifndef HANDMADE_SPRITE_H
#define HANDMADE_SPRITE_H

#include "../handmade_typedefs.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "Vertex.h"
#include "Mesh.h"

namespace Drawing
{
	class Sprite
	{
	public:
		Vector2 Position;
		real32 Depth;
		Vector2 Scale;
		real32 Rotation;
		Mesh *imagePlane;

		Sprite(Vector2 position, real32 depth, Vector2 scale, real32 rotation, Material *material, GLuint shaderProgram) 
			: Position(position), Depth(depth), Scale(scale), Rotation(rotation)
		{
			real32 halfWidth = (real32)material->DiffuseMap->Width / 2.0f;
			real32 halfHeight = (real32)material->DiffuseMap->Height / 2.0f;

			VertexNormalTextureArray *verts = new VertexNormalTextureArray(new VertexNormalTexture[4], 4);

			verts->Vertices[0] = { -halfWidth,  halfHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }; // Top-left
			verts->Vertices[1] = { halfWidth,   halfHeight, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }; // Top-right
			verts->Vertices[2] = { halfWidth,  -halfHeight, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }; // Bottom-right
			verts->Vertices[3] = { -halfWidth, -halfHeight, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }; // Bottom-left

			GLuint *elements = new GLuint[6]{
				2, 1, 0,
				0, 3, 2
			};

			imagePlane = new Mesh(verts, elements, 6, shaderProgram, material);
		}

		Matrix4 GetModelMatrix() const
		{
			return Matrix4::CreateTranslation({ Position.x, Position.y, Depth }) * Matrix4::CreateRotationZ(Rotation) * Matrix4::CreateScale({Scale.x, Scale.y, 1});
		}

		void Draw(const Matrix4 &viewProjection) const
		{
			Matrix4 mvp = viewProjection * GetModelMatrix();
			imagePlane->Draw(mvp, { 1, 1, 1 });
		}
	};
}

#endif // !HANDMADE_SPRITE_H
