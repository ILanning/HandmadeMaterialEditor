#ifndef HANDMADE_MODEL
#define HANDMADE_MODEL

#include "../handmade_typedefs.h"
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "Texture2D.h"
#include "Vertex.h"
#include "Geometry.h"
#include "../libraries/glew.h"

struct Model
{
	Geometry *MeshData;

	Vector3 Size;
	Matrix4 Rotation;
	Vector3 Position;
	Vector3 Color;

	Model(Geometry *meshData, const Vector3 &position = { 0, 0, 0 }, const Vector3 &size = { 1, 1, 1 }, const Matrix4 &rotation = Matrix4::Identity(), const Vector3 &color = { 1, 1, 1 })
		: MeshData(meshData), Position(position), Size(size), Rotation(rotation), Color(color)
	{

	}

	Matrix4 GetModelMatrix()
	{
		Matrix4 result = Matrix4::CreateTranslation(Position) * Rotation * Matrix4::CreateScale(Size);
		return result;
	}

	void Draw(const Matrix4 &view, const Matrix4 &projection)
	{
		Matrix4 mvp = projection * view * GetModelMatrix();
		MeshData->Draw(mvp, Color);
	}
};

#endif