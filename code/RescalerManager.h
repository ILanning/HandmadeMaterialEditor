#ifndef HANDMADE_RESCALERMANAGER_H
#define HANDMADE_RESCALERMANAGER_H

#include "handmade_typedefs.h"
#include "general\Assert.h"
#include "general\StretchyArray.h"
#include "math\Vector2.h"

/**
 *Defines a function that takes an object, and rescales it to a new set of dimensions.  Use this to handle DPI scaling.
*/
#define RESCALEFUNC(name) void name(void *object, Vector2 oldSize, Vector2 newSize, real32 ooomFactor)
typedef RESCALEFUNC(RescaleFunc);

/**
 *Handles window resizes and DPI scaling for the program.  Anything that needs to change on these must register with this class.
*/
class RescalerManager
{
	//TODO: Probably ought to be a singleton

	//TODO: Maybe include a type identifier, tying in with custom reflection system?
	struct ObjectRescalerPair
	{
		void* object;
		RescaleFunc *rescaler;
	};

	//TODO: objects should be unique in this, replace with a hash map later
	StretchyArray<ObjectRescalerPair> rescaleableObjects = StretchyArray<ObjectRescalerPair>();

public:
	void RegisterObject(void *object, RescaleFunc *rescaler)
	{
		ObjectRescalerPair pair = { object, rescaler };
		rescaleableObjects.PushBack(pair);
	}

	void UnregisterObject(void *object)
	{
		Assert(false);//Not implemented
	}

	/** This calls all rescale functions registered with the RescaleManager with the given parameters.
	*/
	void RescaleGraphics(Vector2 oldSize, Vector2 newSize, real32 zoomFactor)
	{
		for (int32 i = 0; i < rescaleableObjects.Length(); i++)
		{
			rescaleableObjects[i].rescaler(rescaleableObjects[i].object, oldSize, newSize, zoomFactor);
		}
	}
};

#endif // !HANDMADE_RESCALERMANAGER_H