#ifndef HANDMADE_SPRITE_H
#define HANDMADE_SPRITE_H

#include "../content/AssetPtr.h"
#include "../math/Rectangle.h"
#include "../handmade_typedefs.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Texture2D.h"

namespace Drawing
{
	///Represents a drawable flat plane image that only rotates about the z axis.  Meant for 2D rendering.
	class Sprite
	{
		Math::Rectangle sampleArea;
		Mesh *imagePlane;

	public:
		///The postion of the center of the Sprite. Defaults to (0, 0).
		Vector2 Position;
		///How far back or forward the Sprite is in the z direction.  Defaults to (0, 0).
		real32 Depth;
		///A size multiplier.  Defaults to (1, 1).
		Vector2 Scale;
		///The clockwise rotation of the sprite, in radians.  Defaults to 0.
		real32 Rotation;

		Sprite(Vector2 position, real32 depth, Vector2 scale, real32 rotation, Math::Rectangle sampleArea, AssetPtr<Material> material, GLuint shaderProgram)
			: Position(position), Depth(depth), Scale(scale), Rotation(rotation)
		{
			real32 texWidth = (real32)material->DiffuseMap->Width;
			real32 texHeight = (real32)material->DiffuseMap->Height;
			real32 uvTop, uvBot, uvLeft, uvRight, halfWidth, halfHeight;

			if (sampleArea.width == -1 && sampleArea.height == -1)
			{
				this->sampleArea = { 0, 0, texWidth, texHeight };

				uvTop = 0;
				uvBot = 1;
				uvLeft = 0;
				uvRight = 1;
				halfWidth = texWidth / 2;
				halfHeight = texHeight / 2;
			}
			else
			{
				this->sampleArea = sampleArea;

				uvTop = sampleArea.y / texHeight;
				uvBot = (sampleArea.y + sampleArea.height) / texHeight;
				uvLeft = sampleArea.x / texWidth;
				uvRight = (sampleArea.x + sampleArea.width) / texWidth;
				halfWidth = sampleArea.width / 2;
				halfHeight = sampleArea.height / 2;
			}

			VertexNormalTextureArray *verts = new VertexNormalTextureArray(new VertexNormalTexture[4], 4);

			(*verts)[0] = { -halfWidth,  halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvLeft, uvBot };
			(*verts)[1] = { halfWidth,   halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvRight, uvBot };
			(*verts)[2] = { halfWidth,  -halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvRight, uvTop };
			(*verts)[3] = { -halfWidth, -halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvLeft, uvTop };

			GLuint *elements = new GLuint[6]{
				2, 1, 0,
				0, 3, 2
			};

			imagePlane = new Mesh(verts, elements, 6, shaderProgram, material);
		}

		Sprite(AssetPtr<Material> material, GLuint shaderProgram) : Sprite({ 0, 0 }, 0, { 1, 1 }, 0, { -1, -1, -1, -1 }, material, shaderProgram) {}

		///Get the size and location of the current sample area.
		Math::Rectangle GetSampleArea() const
		{
			return sampleArea;
		}

		///Set what section of the texture is used (in pixels) and changes the mesh of the sprite to match.  Note that this may change the dimensions of the sprite.
		void SetSampleArea(const Math::Rectangle area)
		{
			sampleArea = area;

			Texture2D &tex = *imagePlane->MeshMaterial->DiffuseMap;
			VertexNormalTextureArray &verts = *(VertexNormalTextureArray*)(void*)imagePlane->Vertices;

			real32 uvTop = area.y / tex.Height;
			real32 uvBot = (area.y + area.height) / tex.Height;
			real32 uvLeft = area.x / tex.Width;
			real32 uvRight = (area.x + area.width) / tex.Width;
			real32 halfWidth = area.width / 2;
			real32 halfHeight = area.height / 2;

			verts[0] = { -halfWidth,  halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvLeft, uvBot };
			verts[1] = { halfWidth,   halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvRight, uvBot };
			verts[2] = { halfWidth,  -halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvRight, uvTop };
			verts[3] = { -halfWidth, -halfHeight, 0.0f, 0.0f, 0.0f, 1.0f,  uvLeft, uvTop };

			imagePlane->UpdateBufferObjects();
		}

		///Get the model matrix for this Sprite.
		Matrix4 GetModelMatrix() const
		{
			return Matrix4::CreateTranslation({ Position.x, Position.y, Depth }) * Matrix4::CreateRotationZ(Rotation) * Matrix4::CreateScale({ Scale.x, Scale.y, 1 });
		}

		///Draw this Sprite with the given View Projection matrix.
		void Draw(const Matrix4 &viewProjection) const
		{
			Matrix4 mvp = viewProjection * GetModelMatrix();
			imagePlane->Draw(mvp, { 1, 1, 1 });
		}
	};
}

#endif // !HANDMADE_SPRITE_H
