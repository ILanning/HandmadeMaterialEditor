#ifndef HANDMADE_STRETCHY_ARRAY
#define HANDMADE_STRETCHY_ARRAY

#include "../handmade_typedefs.h"

template <class T>

struct StretchyArrayNode
{
	//Fits each node near exactly into the standard page size for Windows/Linux/OSX
	static const int32 Capacity = (4096 - sizeof(StretchyArrayNode*)) / sizeof(T);
	T Items[Capacity];
	StretchyArrayNode *Next = nullptr;
};

template <class T>
/**
 * \brief An array-accessible data structure that extends itself automatically.
*/
class StretchyArray
{
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

	StretchyArray& StretchyArray::operator=(StretchyArray arg)
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
				last = last->Next;
			}
			lastSection++;
		}
		last->Items[nextEmpty % StretchyArrayNode<T>::Capacity] = value;
		nextEmpty++;

		return nextEmpty - 1;
	}

	/**
		/brief Adds `count` objects in the passed-in array to the StretchyArray.
	*/
	int32 PushBackMany(const T *items, int32 count)
	{
		for (int32 i = 0; i < count; i++)
		{
			PushBack(items[i]);
		}
		return nextEmpty - 1;
	}

	/**
		/brief Adds all - or optionally `count` - objects in the passed-in StretchyArray to this one.
	*/
	int32 PushBackMany(StretchyArray<T> items, int32 count = -1)
	{
		if (count == -1)
		{
			count = items.Length();
		}
		for (int32 i = 0; i < count; i++)
		{
			PushBack(items[i]);
		}
		return nextEmpty - 1;

	}

	int32 Length()
	{
		return nextEmpty;
	}

	T *ToArray()
	{
		//TODO(Ian):  Fill this out, use memcpy() probably
		T *result = new T[nextEmpty];

		int32 sectionSize = StretchyArrayNode<T>::Capacity;
		StretchyArrayNode<T> *curr = head;
		for (int i = 0; i < lastSection - 1; i++)
		{
			memcpy(result + i * sectionSize, curr, sectionSize * sizeof(T));
			curr = curr->Next;
		}
		memcpy(result + (lastSection - 1) * sectionSize, curr, (nextEmpty % sectionSize) * sizeof(T));

		return result;
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

#endif