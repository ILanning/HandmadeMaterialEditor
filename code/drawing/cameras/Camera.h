#ifndef HANDMADE_CAMERA
#define HANDMADE_CAMERA

#include "..\..\math\Matrix4.h"
#include "..\..\math\Quaternion.h"
#include "..\..\math\Vector3.h"
#include "..\..\input\InputManager.h"

namespace Drawing
{
	class Camera
	{
		Matrix4 projection;

	public:
		/** Returns a unit vector representing the direction that the camera is facing.		
		*/
		virtual Vector3 GetLookVector() const = 0;

		/** Returns the point in space that the camera is looking at.
		*/
		virtual Vector3 GetLookAtPosition() const = 0;
		/** Sets the point in space that the camera is looking at.
		*/
		virtual void SetLookAtPosition(const Vector3 &newPosition) = 0;

		/** Returns the position of the camera.
		*/
		virtual Vector3 GetCameraPosition() const = 0;
		/** Sets the position of the camera.
		*/
		virtual void SetCameraPosition(const Vector3 &newPosition) = 0;

		/** Returns the distance from the camera to the point that it's looking at.
		*/
		virtual real32 GetZoom() const = 0;
		/** Sets the distance from the camera to the point that it's looking at.
		*/
		virtual void SetZoom(real32 newZoom) = 0;

		/** Returns the projection matrix for this camera.
		*/
		virtual Matrix4 GetProjection() const
		{
			return projection;
		}
		/** Sets the projection matrix for this camera.
		*/
		virtual void SetProjection(const Matrix4 &newProjection)
		{
			projection = newProjection;
		}

		/** Returns the current view matrix for this camera.
		*/
		virtual Matrix4 GetView() const = 0;
		/** Returns the view matrix multiplied by the projection matrix.
		*/
		virtual Matrix4 GetViewProjection() const
		{
			return projection * GetView();
		}


		/** Rotates the camera by the specified quaternion.
		*/
		virtual void Rotate(const Quaternion &spin) = 0;

		/** Allows the camera to take input from the user.
		*/
		virtual void HandleInput(const Input::InputManager &manager) = 0;

		/** Allows the camera to update itself.  For instance, to apply movement smoothing.
		*/
		virtual void Update() = 0;
	};
}

#endif