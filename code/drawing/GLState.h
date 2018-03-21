#ifndef HANDMADE_GLSTATE
#define HANDMADE_GLSTATE

#include "../math/Matrix4.h"
#include "../handmade_typedefs.h"
#include "../libraries/glew.h"

namespace Drawing
{
	struct GLState
	{
		Matrix4 View;
		Matrix4 Projection;

		GLState();

		void SetView(const Matrix4 &view);
		void SetProjection(const Matrix4 &projection);
	};

	GLState::GLState()
	{
		View = Matrix4::Identity();
		Projection = Matrix4::Identity();
	}

	void GLState::SetView(const Matrix4 &view)
	{
		this->View = view;
	}

	void GLState::SetProjection(const Matrix4 &projection)
	{
		this->Projection = projection;
	}
}

#endif