#ifndef HANDMADE_GENERAL_SHAREDPTR_H
#define HANDMADE_GENERAL_SHAREDPTR_H

#include "..\handmade_typedefs.h"

template<typename T>
class SharedPtr
{
	int32* counter;
	T* item;

public:
	SharedPtr(T* item) : item(item)
	{
		counter = new int32(1);
	}

	SharedPtr(const SharedPtr& other)
	{
		(*other.counter)++;
		counter = other.counter;
		item = other.item;
	}

	SharedPtr& operator=(SharedPtr other)
	{
		(*other.counter)++;
		counter = other.counter;
		item = other.item;
	}

	T& operator*()
	{
		return *item;
	}

	T* operator->()
	{
		return item;
	}

	~SharedPtr()
	{
		*counter--;
		if (*counter == 0)
		{
			delete counter;
			delete item;
		}
	}
};

#endif //HANDMADE_GENERAL_SHAREDPTR_H