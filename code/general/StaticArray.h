#ifndef HANDMADE_STATICARRAY
#define HANDMADE_STATICARRAY

#include "../handmade_typedefs.h"

template<typename T>
struct StaticArray
{
	uint32 Length = 0;
	T* Data = nullptr;

	StaticArray() {}
	StaticArray(T* data, uint32 size) : Length(size), Data(data) {}

	T& operator[] (uint32 value) { return Data[value]; }
	T operator[] (uint32 value) const { return Data[value]; }
};

#endif //HANDMADE_STATICARRAY