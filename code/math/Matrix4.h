#ifndef HANDMADE_MATRIX4_H
#define HANDMADE_MATRIX4_H

#include <math.h>
#include "..\handmade_typedefs.h"
#include "Vector2.h"
#include "Vector3.h"

///A three dimensional matrix.
struct Matrix4
{
	///The number of rows in the matrix
	const static int32 RowCount = 4;
	///The number of elements in a matrix
	const static int32 ElementCount = RowCount * RowCount;
	union
	{
		struct
		{
			real32 m00;
			real32 m01;
			real32 m02;
			real32 m03;
			real32 m10;
			real32 m11;
			real32 m12;
			real32 m13;
			real32 m20;
			real32 m21;
			real32 m22;
			real32 m23;
			real32 m30;
			real32 m31;
			real32 m32;
			real32 m33;
		};
		real32 m[4][4];
		real32 elements[16];
	};

	Matrix4 Inverse();
	real32 Determinant();
	Matrix4 Cofactor();
	Matrix4 Transpose();
	void SetTranslation(const Vector3 &a);
	Vector3 Transform(const Vector3 &a);

	friend Matrix4 operator+(const Matrix4& a, const Matrix4& b);
	friend Matrix4 operator+(const Matrix4& a, real32 b);
	friend Matrix4 operator-(const Matrix4& a, const Matrix4& b);
	friend Matrix4 operator-(const Matrix4& a, real32 b);
	friend Matrix4 operator*(const Matrix4& a, const Matrix4& b);
	friend Matrix4 operator*(const Matrix4& a, real32 b);
	friend Matrix4 operator/(const Matrix4& a, real32 b);

	friend Matrix4 &operator+=(Matrix4& a, const Matrix4& b);
	friend Matrix4 &operator+=(Matrix4& a, real32 b);
	friend Matrix4 &operator-=(Matrix4& a, const Matrix4& b);
	friend Matrix4 &operator-=(Matrix4& a, real32 b);
	friend Matrix4 &operator*=(Matrix4& a, const Matrix4& b);
	friend Matrix4 &operator*=(Matrix4& a, real32 b);
	friend Matrix4 &operator/=(Matrix4& a, real32 b);

	friend bool operator==(const Matrix4& a, const Matrix4& b);
	friend bool operator!=(const Matrix4& a, const Matrix4& b);

	static Matrix4 Identity()
	{
		Matrix4 result = { 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
		return result;
	}

	static Matrix4 CreateRotation(const Vector3 &radianAngles)
	{
		return CreateRotationX(radianAngles.x) * CreateRotationY(radianAngles.y) * CreateRotationZ(radianAngles.z);
	}

	static Matrix4 CreateRotationX(real32 radians)
	{
		real32 r = -radians;
		Matrix4 result = { 1, 0, 0, 0,
						   0, cosf(r), -sinf(r), 0,
						   0, sinf(r),  cosf(r), 0,
						   0, 0, 0, 1 };
		return result;
	}

	static Matrix4 CreateRotationY(real32 radians)
	{
		real32 r = -radians;
		Matrix4 result = { cosf(r), 0, sinf(r), 0,
						   0, 1, 0, 0,
						  -sinf(r), 0, cosf(r), 0,
						   0, 0, 0, 1 };
		return result;
	}

	static Matrix4 CreateRotationZ(real32 radians)
	{
		real32 r = -radians;
		Matrix4 result = { cosf(r), -sinf(r), 0, 0,
						   sinf(r),  cosf(r), 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 };
		return result;
	}

	static Matrix4 CreateScale(real32 x, real32 y, real32 z)
	{
		Matrix4 result = { x, 0, 0, 0,
						   0, y, 0, 0,
						   0, 0, z, 0,
						   0, 0, 0, 1 };
		return result;
	}

	static Matrix4 CreateScale(const Vector3 &scaleFactor)
	{
		return CreateScale(scaleFactor.x, scaleFactor.y, scaleFactor.z);
	}

	static Matrix4 CreateTranslation(const Vector3 &position)
	{
		Matrix4 result = { 1, 0, 0, position.x,
						   0, 1, 0, position.y,
					   	   0, 0, 1, position.z,
						   0, 0, 0, 1 };
		return result;
	}

	static Matrix4 CreatePerspective(real32 fov, real32 aspectRatio, real32 nearPlane, real32 farPlane)
	{
		/*Resources: 
		https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix
		http://www.songho.ca/opengl/gl_projectionmatrix.html
		*/

		real32 negativeDepth = nearPlane - farPlane;
		float fovScale = 1.0f / tanf(fov / 2);

		Matrix4 result = { fovScale / aspectRatio,  0, 0, 0,
						   0, fovScale, 0, 0,
						   0, 0, farPlane / negativeDepth, (nearPlane * farPlane) / negativeDepth,
						   0, 0, -1, 0 };
		return result;
	}

	static void RescalePerspective(void *rawMatrixPointer, Vector2 newSize)
	{
		Matrix4 &matrix = *(Matrix4 *)rawMatrixPointer;
		real32 newAspect = newSize.x / newSize.y;
		matrix.m00 = matrix.m11 / newAspect;
	}

	static Matrix4 CreateOrthographic(real32 screenWidth, real32 screenHeight, real32 nearPlane, real32 farPlane)
	{
		real32 negativeDepth = nearPlane - farPlane;
		/*Matrix4 result = {};
		result.m00 = 2.0f / screenWidth;
		//result.M12 = result.M13 = result.M14 = 0.0f;
		result.m11 = 2.0f / screenHeight;
		//result.M21 = result.M23 = result.M24 = 0.0f;
		result.m22 = 1.0f / depth;
		//result.M31 = result.M32 = result.M34 = 0.0f;
		//result.M41 = result.M42 = 0.0f;
		result.m32 = nearPlane / depth;
		result.m33 = 1.0f;*/

		Matrix4 result = {
			2.0f / screenWidth,  0, 0, 0,
			0, 2.0f / screenHeight, 0, 0,
			0, 0, 2.0f / negativeDepth, 0,
			0, 0, 0, 1
		};

		return result;
		/*real32 halfWidth = screenWidth / 2;
		real32 halfHeight = screenHeight / 2;
		real32 depth = farPlane - nearPlane;
		Matrix4 result = { 
			1 / halfWidth,  0, 0, 0,
			0, 1 / halfHeight, 0, 0,
			1, 1, -(2 / depth), 0,
			0, 0, -((farPlane + nearPlane) / depth), 1 };
		return result;*/
	}

	static void RescaleOrthographic(void *rawMatrixPointer, Vector2 oldSize, Vector2 newSize, real32 zoomFactor)
	{
		Matrix4 &matrix = *(Matrix4 *)rawMatrixPointer;
		matrix.m00 = 2.0f / newSize.x;
		matrix.m11 = 2.0f / newSize.y;
	}
};

#endif //HANDMADE_MATRIX4_H