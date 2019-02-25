#ifndef HANDMADE_STRETCHYARRAY_H
#define HANDMADE_STRETCHYARRAY_H

#include "../handmade_typedefs.h"
#include "Assert.h"

/// An array-accessible data structure that extends itself automatically.
template <class T>
class StretchyArray
{
	template <class T>
	struct StretchyArrayNode
	{
		//Fits each node near exactly into the standard page size for Windows/Linux/OSX
		static const int32 Capacity = (4096 - sizeof(StretchyArrayNode*) * 2) / sizeof(T);
		T Items[Capacity];
		StretchyArrayNode *Next = nullptr;
		StretchyArrayNode *Prev = nullptr;
	};

	StretchyArrayNode<T> *head = nullptr;
	StretchyArrayNode<T> *last = nullptr;
	int32 lastSection = 0;
	int32 nextEmpty = 0;

public:
	StretchyArray()
	{
		//Does not support objects larger than 4088 bytes
		StaticAssert(StretchyArrayNode<T>::Capacity > 0);
	}

	/// Swaps in memory two StretchyArrays.
	void Swap(StretchyArray &other)
	{
		StretchyArrayNode<T> *tempPointer = head;
		head = other.head;
		other.head = tempPointer;
		tempPointer = last;
		last = other.last;
		other.last = tempPointer;

		int32 tempNum = lastSection;
		lastSection = other.lastSection;
		other.lastSection = tempNum;
		tempNum = nextEmpty;
		nextEmpty = other.nextEmpty;
		other.nextEmpty = tempNum;
	}

	/// Returns the total number of items currently in the StretchyArray.
	int32 Length()
	{
		return nextEmpty;
	}

	/// Adds a new item to the back of the StretchyArray.
	int32 PushBack(const T &value)
	{
		if (nextEmpty == lastSection * StretchyArrayNode<T>::Capacity)
		{
			if (head == nullptr)
			{
				head = new StretchyArrayNode<T>();
				last = head;
			}
			else
			{
				last->Next = new StretchyArrayNode<T>();
				last->Next->Prev = last;
				last = last->Next;
			}
			lastSection++;
		}
		last->Items[nextEmpty % StretchyArrayNode<T>::Capacity] = value;
		nextEmpty++;

		return nextEmpty - 1;
	}

	/// Adds `count` objects in the passed-in array to the StretchyArray.
	int32 PushBackMany(const T *items, int32 count)
	{
		for (int32 i = 0; i < count; i++)
		{
			PushBack(items[i]);
		}
		return nextEmpty - 1;
	}

	/** Adds all (or optionally `count`) objects in the passed-in StretchyArray to this one.
	 * @param items The StretchyArray to pull from.
	 * @param count The number of items to pull.  Pass in -1 to add everything.
	 * @param offset The position to begin adding items from.
	*/
	int32 PushBackMany(StretchyArray<T> items, int32 count = -1, int32 offset = 0)
	{		
		Assert(count + offset <= items.Length());

		if (count == -1)
		{
			count = items.Length();
		}
		for (int32 i = offset; i < count + offset; i++)
		{
			PushBack(items[i]);
		}
		return nextEmpty - 1;
	}

	/// Removes an item from the back of the StretchyArray.
	void PopBack()
	{
		if (nextEmpty > 0)
		{
			nextEmpty--;
			//TODO: Add hysterisis of some sort, probably just allowing for an empty node
			if (nextEmpty < (lastSection - 1) * StretchyArrayNode<T>::Capacity)
			{
				lastSection--;
				StretchyArrayNode<T> *toDelete = last;
				last = last->Prev;
				last->Next = nullptr;
				delete toDelete;
			}
		}
	}

	/// Removes `count` items from the back of the StretchyArray.
	void PopBackMany(int32 count)
	{
		if (nextEmpty > count - 1)
		{
			//TODO: Fix bug - this function will not destroy objects at the expected time
			nextEmpty -= count;
			int32 sectionDeletes = lastSection - (nextEmpty - count) / StretchyArrayNode<T>::Capacity;
			for (; sectionDeletes > 0; sectionDeletes--)
			{
				lastSection--;
				StretchyArrayNode<T> *toDelete = last;
				last = last->Prev;
				last->Next = nullptr;
				delete toDelete;
			}
		}
	}

	/// Returns a normal contiguous array containing all items in the StretchyArray.
	T *ToArray()
	{
		//CONSIDER: Can't use memcpy() due to move semantics, maybe make an alternate class/template specializatiosn for that?
		T *result = new T[nextEmpty];

		int32 sectionSize = StretchyArrayNode<T>::Capacity;
		StretchyArrayNode<T> *curr = head;
		for (int sectionIter = 0; sectionIter < lastSection - 1; sectionIter++)
		{
			for (int32 i = 0; i < sectionSize; i++)
			{
				result[sectionIter * sectionSize + i] = curr->Items[i];
			}
			curr = curr->Next;
		}

		for (int32 i = 0; i < nextEmpty % sectionSize; i++)
		{
			result[(lastSection - 1) * sectionSize + i] = curr->Items[i];
		}

		return result;
	}

	StretchyArray& operator=(StretchyArray arg)
	{
		Swap(arg);
		return *this;
	}

	T& operator[] (int32 x)
	{
		int32 section = 1;
		StretchyArrayNode<T> *currNode = head;
		while (x > section * StretchyArrayNode<T>::Capacity - 1 && currNode)
		{
			section++;
			currNode = currNode->Next;
		}

		Assert(currNode != nullptr);

		return currNode->Items[x % StretchyArrayNode<T>::Capacity];
	}

	~StretchyArray()
	{
		if (head)
		{
			StretchyArrayNode<T> *next = head->Next;
			while (next)
			{
				StretchyArrayNode<T> *toDelete = next;
				next = next->Next;
				delete toDelete;
			}
			delete head;
			head = nullptr;
			last = nullptr;
		}
		nextEmpty = 0;
		lastSection = 0;
	}
};

#endif //HANDMADE_STRETCHYARRAY_H