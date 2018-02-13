#ifndef HANDMADE_RESCALERMANAGER_H
#define HANDMADE_RESCALERMANAGER_H

#include "handmade_typedefs.h"
#include "general\Assert.h"
#include "general\StretchyArray.h"
#include "math\Vector2.h"

#define RESCALEFUNC(name) void name(void *object, Vector2 oldSize, Vector2 newSize)
typedef RESCALEFUNC(RescaleFunc);

struct RescalerManager
{
	//TODO: Maybe include a type identifier, tying in with custom reflection system?
	struct ObjectRescalerPair
	{
		void* object;
		RescaleFunc *rescaler;
	};

	//TODO(Ian): objects should be unique in this, replace with a hash map later
	StretchyArray<ObjectRescalerPair> rescaleableObjects = StretchyArray<ObjectRescalerPair>();

	void RegisterObject(void *object, RescaleFunc *rescaler)
	{
		ObjectRescalerPair pair = { object, rescaler };
		rescaleableObjects.PushBack(pair);
	}

	void UnregisterObject(void *object)
	{
		Assert(false);//Not implemented
	}

	/** Responsible for DPI rescaling and window dimension changes.
	*/
	void RescaleGraphics(Vector2 oldSize, Vector2 newSize)
	{
		for (int32 i = 0; i < rescaleableObjects.Length(); i++)
		{
			rescaleableObjects[i].rescaler(rescaleableObjects[i].object, oldSize, newSize);
		}
	}
};

#endif // !HANDMADE_RESCALERMANAGER_H