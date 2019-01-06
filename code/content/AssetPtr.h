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
	AssetPtr() {}

	AssetPtr(T* item, Content::AssetPtrSharedData* groupData) : item(item), groupData(groupData)
	{
		groupData->Count++;
	}

	AssetPtr(const AssetPtr& other)
	{
		groupData = other.groupData;
		groupData->Count++;
		item = other.item;
	}

	AssetPtr& operator=(AssetPtr other)
	{
		groupData = other.groupData;
		groupData->Count++;
		item = other.item;
		return *this;
	}

	bool IsNull() const
	{
		return item == nullptr;
	}

	T* GetRaw() const
	{
		return item;
	}

	T& operator*() const
	{
		return *item;
	}

	T* operator->() const
	{
		return item;
	}

	~AssetPtr()
	{
		if (groupData)
		{
			groupData->Count--;
		}
	}
};

#endif //HANDMADE_CONTENT_ASSETPTR_H