/*
List that owns its own memory and resizes as needed
*/
#ifndef HANDMADE_ARRAYLIST_H
#define HANDMADE_ARRAYLIST_H

#include "../handmade_typedefs.h"
#include "../handmade_funcdefs.h"
#include "Assert.h"

template <class T>
struct ArrayList
{
	T *internalArray = nullptr;
	int32 arraySize = 0;
	int32 count = 0;
	float upsizeFactor = 2.0f;
	float downsizeFactor = 0.4f;
	AllocMemoryFunc *memAlloc = nullptr;
	DeallocMemoryFunc *memFree = nullptr;

private:
	bool resizeUp(int32 newSize, int32 gapIndex = 0, int32 gapCount = 0)
	{
		if (newSize == arraySize)
		{
			return true;
		}
		//CONSIDER(IAN): Template specializations for types that can be memcpy'd, see if that improves performance
		T* newArray = (T*)memAlloc(newSize * sizeof(T));
		//TODO(Ian): Assert on debug build, return false otherwise
		//           Or maybe contain that to the actual memory functions?
		Assert(newArray != nullptr);
		if (internalArray)
		{
			for (int32 internalIter = 0, newIter = 0; internalIter < count; internalIter++, newIter++)
			{
				if (newIter == gapIndex)
				{
					newIter += gapCount;
				}
				newArray[newIter] = internalArray[internalIter];
			}
		}
		internalArray = newArray;
		arraySize = newSize;
		return true;
	}

	bool resizeDown(int32 newSize, int32 deleteIndex = 0, int32 deleteCount = 0)
	{
		if (newSize == arraySize)
		{
			return true;
		}
		//CONSIDER(IAN): Template specializations for types that can be memcpy'd, see if that improves performance
		T* newArray = (T*)memAlloc(newSize * sizeof(T));
		//TODO(Ian): Assert on debug build, return false otherwise
		//           Or maybe contain that to the actual memory functions?
		Assert(newArray != nullptr);
		if (internalArray)
		{
			for (int32 internalIter = 0, newIter = 0; internalIter < count; internalIter++, newIter++)
			{
				if (internalIter == deleteIndex)
				{
					internalIter += deleteCount;
				}
				newArray[newIter] = internalArray[internalIter];
			}
		}
		internalArray = newArray;
		arraySize = newSize;
		return true;
	}

public:	
	ArrayList(AllocMemoryFunc memoryAllocator, DeallocMemoryFunc *memoryDeallocator, int starterCapacity = 0) 
		: memAlloc(memoryAllocator), memFree(memoryDeallocator)
	{
		Assert(memAlloc != nullptr);
		Assert(memFree != nullptr);

		if (starterCapacity != 0)
		{
			internalArray = (T*)(memAlloc(sizeof(T) * starterCapacity));
		}
	}

	bool Add(T item)
	{
		if (count == arraySize)
		{
			resize((int32)(arraySize * upSizeFactor));
			Assert(count != arraySize);
		}
		internalArray[count] = item;
		count++;
	}

	bool AddRange(T *items, int32 count)
	{
		if (this->count + count > arraySize)
		{
			int32 newSize = (int32)(arraySize * upSizeFactor);
			while (this->count + count > arraySize)
			{
				newSize = (int32)(newSize * upSizeFactor);
			}
			resize(newSize);
		}
		for()
	}

	bool Insert(T item, int32 index)
	{

	}

	bool InsertRange(T *items, int32 index, int32 count)
	{

	}

	void Delete(int32 index)
	{
		//
		for (; index < count - 1; index++)
		{
			T temp = internalArray[index]; 
			internalArray[index] = internalArray[index + 1];
			intenalArray[index + 1] = temp;
		}
		index[count - 1] = T();
		count--;
	}

	void DeleteRange(int32 startIndex, int32 count)
	{

	}

	T* CopyRange(AllocMemoryFunc alloc, int32 startIndex, int32 count)
	{

	}

	T* ToArray(AllocMemoryFunc alloc)
	{
		return CopyRange(alloc, 0, count);
	}
};

#endif // !HANDMADE_ARRAYLIST_H
