#ifndef HANDMADE_ARRAYLIST_H
#define HANDMADE_ARRAYLIST_H

#include "../handmade_typedefs.h"
#include "../handmade_funcdefs.h"
#include "Assert.h"

template <class Item, class Allocator>
/*
List that owns its own memory and resizes as needed
*/
class ArrayList
{
public:
	float UpsizeFactor = 2.0f;
	float DownsizeFactor = 0.0f;

private:
	Item *internalArray = nullptr;
	int32 arraySize = 0;
	int32 count = 0;
	Allocator *memory = nullptr;
	static const int32 baseSize = 4;

public:
	ArrayList(Allocator *allocator, int starterCapacity = 0)
		: memory(allocator), arraySize(starterCapacity)
	{
		Assert(memory != nullptr);

		if (starterCapacity != 0)
		{
			internalArray = memory->Allocate<Item>(starterCapacity);
		}
	}

	Item operator [] (int32 i) const
	{
		return internalArray[i];
	}

	Item& operator [] (int32 i)
	{
		return internalArray[i];
	}

	bool Add(Item item)
	{
		if (count == arraySize)
		{
			bool success;
			if (arraySize != 0)
			{
				success = resizeUp((int32)(arraySize * UpsizeFactor));
			}
			else
			{
				success = resizeUp(baseSize);
			}

			if (!success)
			{
				return false;
			}
		}
		internalArray[count] = item;
		count++;

		return true;
	}

	bool AddRange(Item *items, int32 arrayCount)
	{
		bool success;

		if (arraySize == 0)
		{
			success = resizeUp(arrayCount > baseSize ? arrayCount : baseSize);
		}
		else if (count + arrayCount > arraySize)
		{
			int32 newSize = (int32)(arraySize * UpsizeFactor);
			while (count + arrayCount > newSize)
			{
				newSize = (int32)(newSize * UpsizeFactor);
			}
			success = resizeUp(newSize);
		}
		else
		{
			success = true;
		}

		if (!success)
		{
			return false;
		}

		for (int32 i = 0; i < arrayCount; i++)
		{
			internalArray[count + i] = items[i];
		}
		count += arrayCount;

		return true;
	}

	bool Insert(Item item, int32 index)
	{
		Assert(index >= 0);

		bool success = true;
		int32 newCount = count > index ? count + 1 : index + 1;

		if (arraySize > count && arraySize > index)
		{
			if (index < count - 1)
			{
				for (int32 i = count; i >= index; i--)
				{
					internalArray[i - 1] = internalArray[i];
				}
			}
		}
		else if (arraySize == 0)
		{
			success = resizeUp(index > baseSize ? index : baseSize);
		}
		else
		{
			success = resizeUp(nextSizeUp(newCount), index, 1);
		}

		if (!success)
		{
			return false;
		}

		internalArray[index] = item;
		count = newCount;

		return true;
	}

	bool InsertRange(Item *items, int32 index, int32 arrayCount)
	{
		Assert(arrayCount >= 0);
		Assert(index >= 0);

		bool success;
		int32 newCount = count > index ? count + arrayCount : index + arrayCount;

		if (arrayCount == 0)
		{
			return true;
		}

		if ((index < count && arraySize >= count + arrayCount) ||
			(arraySize > index + arrayCount))
		{
			if (index < count)
			{
				for (int32 i = count - 1; i >= index; i--)
				{
					internalArray[i + arrayCount] = internalArray[i];
				}
			}

			success = true;
		}
		else if (arraySize == 0)
		{
			success = resizeUp(newCount > baseSize ? newCount : baseSize);
		}
		else
		{
			success = resizeUp(nextSizeUp(newCount), index, arrayCount);
		}

		if (!success)
		{
			return false;
		}

		for (int32 i = 0; i < arrayCount; i++)
		{
			internalArray[i + index] = items[i];
		}
		count = newCount;

		return true;
	}

	void Delete(int32 index)
	{
		DeleteRange(index, 1);
	}

	void DeleteRange(int32 index, int32 deleteCount)
	{
		Assert(index >= 0);
		Assert(deleteCount >= 0);

		if (index >= count)
		{
			return;
		}
		if (index + deleteCount > count)
		{
			deleteCount -= index + deleteCount - count;
		}

		if (DownsizeFactor != 0.0f && (count - deleteCount) / arraySize < DownsizeFactor)
		{
			resizeDown(nextSizeDown(count - deleteCount), index, deleteCount);
		}
		else
		{
			int32 i = index;
			for (; i < count - deleteCount; i++)
			{
				internalArray[i] = internalArray[i + deleteCount];
			}
			for (; i < count; i++)
			{
				internalArray[i] = Item();
			}
		}

		count -= deleteCount;
	}

	template <class ArrayAllocator>
	Item* RangeToArray(ArrayAllocator *alloc, int32 startIndex, int32 arrayCount)
	{
		Assert(startIndex >= 0);
		Assert(startIndex + arrayCount <= count);

		if (arrayCount == 0)
		{
			return nullptr;
		}

		Item *resultArray = alloc->Allocate<Item>(arrayCount);

		Assert(resultArray != nullptr);

		for (int32 i = 0; i < arrayCount; i++)
		{
			resultArray[i] = internalArray[startIndex + i];
		}

		return resultArray;
	}

	template <class ArrayAllocator>
	Item* ToArray(ArrayAllocator *alloc)
	{
		return RangeToArray(alloc, 0, count);
	}

	int32 Count() const { return count; }
	int32 Capacity() const { return arraySize; }

private:

	inline int32 nextSizeUp(int32 targetCount)
	{
		double newSize = arraySize;
		while (targetCount > newSize)
		{
			newSize *= UpsizeFactor;
		}
		return (int32)newSize;
	}

	inline int32 nextSizeDown(int32 targetCount)
	{
		double newSize = arraySize;
		while (targetCount < newSize && newSize > baseSize)
		{
			newSize *= DownsizeFactor;
		}
		newSize /= DownsizeFactor;

		return (int32)newSize;
	}

	bool resizeUp(int32 newSize, int32 gapIndex = 0, int32 gapCount = 0)
	{
		if (newSize == arraySize)
		{
			return true;
		}
		//CONSIDER(IAN): Template specializations for types that can be memcpy'd, see if that improves performance
		Item* newArray = memory->Allocate<Item>(newSize);
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

		memory->Deallocate(internalArray);
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
		Item* newArray = memory->Allocate<Item>(newSize);
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

		memory->Deallocate(internalArray);
		internalArray = newArray;
		arraySize = newSize;
		return true;
	}
};

#endif // !HANDMADE_ARRAYLIST_H
