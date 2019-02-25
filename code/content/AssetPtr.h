#ifndef HANDMADE_CONTENT_ASSETPTR_H
#define HANDMADE_CONTENT_ASSETPTR_H

#include "..\handmade_typedefs.h"
#include "AssetPtrSharedData.h"

template<typename T>
class AssetPtr
{
	Content::AssetPtrSharedData* groupData = nullptr;
	T* item = nullptr;

public:
	inline AssetPtr() {}

	inline AssetPtr(T* item, Content::AssetPtrSharedData* groupData) : item(item), groupData(groupData)
	{
		groupData->Count++;
	}

	inline AssetPtr(const AssetPtr& other)
	{
		groupData = other.groupData;
		groupData->Count++;
		item = other.item;
	}

	inline AssetPtr& operator=(AssetPtr other)
	{
		groupData = other.groupData;
		groupData->Count++;
		item = other.item;
		return *this;
	}

	inline bool IsNull() const
	{
		return item == nullptr;
	}

	inline T* GetRaw() const
	{
		return item;
	}

	inline T& operator*() const
	{
		return *item;
	}

	inline T* operator->() const
	{
		return item;
	}

	inline ~AssetPtr()
	{
		if (groupData)
		{
			groupData->Count--;
		}
	}
};

#endif //HANDMADE_CONTENT_ASSETPTR_H