#ifndef HANDMADE_CONTENT_ASSETPTRSHAREDDATA_H
#define HANDMADE_CONTENT_ASSETPTRSHAREDDATA_H

#include "..\handmade_typedefs.h"

namespace Content
{
	class AssetPtrSharedData
	{
	public:
		int32 Count = 0;

		AssetPtrSharedData() {}
		AssetPtrSharedData(int32 count) : Count(count) {}
	};
}
#endif //HANDMADE_CONTENT_ASSETPTRSHAREDDATA_H