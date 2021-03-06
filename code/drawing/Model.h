#ifndef HANDMADE_MODEL_H
#define HANDMADE_MODEL_H

#include "../handmade_typedefs.h"
#include "../content/AssetPtr.h"
#include "../content/MeshCollection.h"
#include "../general/ArrayList.h"
#include "../general/memory/NewDeleteArena.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "Texture2D.h"
#include "../libraries/glew.h"

namespace Drawing
{
	///Represents a model that might be displayed in the world.
	struct Model
	{
		///The meshes that the model draws.
		AssetPtr<Content::MeshCollection> Meshes = {};
		///The scale factor applied to the model.
		Vector3 Size = {1, 1, 1};
		///The way that the model is rotated in space.
		Matrix4 Rotation = Matrix4::Identity();
		///The location that the model is drawn at.
		Vector3 Position = {};
		///The color applied to the model.
		Vector3 Color = {1, 1, 1};
		///The offset from the position at which the model is drawn.
		Vector3 Pivot = {};

		Model(AssetPtr<Content::MeshCollection> meshes, const Vector3 &position = { 0, 0, 0 }, const Vector3 &size = { 1, 1, 1 }, const Matrix4 &rotation = Matrix4::Identity(), const Vector3 &color = { 1, 1, 1 })
			: Meshes(meshes), Position(position), Size(size), Rotation(rotation), Color(color)
		{

		}

		///Returns a matrix representing all the transformations applied to this model
		Matrix4 GetModelMatrix() const
		{
			Matrix4 result = Matrix4::CreateTranslation(Position) * Rotation * Matrix4::CreateScale(Size) * Matrix4::CreateTranslation(Pivot);
			return result;
		}

		///Draws the model to the world.
		void Draw(const Matrix4 &viewProjection) const
		{
			Matrix4 mvp = viewProjection * GetModelMatrix();
			for (int32 i = 0; i < Meshes->Length(); i++)
			{
				(*Meshes)[i].Draw(mvp, Color);
			}
		}
	};
}

#endif //HANDMADE_MODEL_H