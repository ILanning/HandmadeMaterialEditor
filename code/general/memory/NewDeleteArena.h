#ifndef HANDMADE_MEMORY_NEWDELETEARENA_H
#define HANDMADE_MEMORY_NEWDELETEARENA_H

#ifdef HANDMADE_ALLOW_NEW

#include "../../handmade_typedefs.h"

namespace Memory
{
	///Memory arena that wraps the new/delete functions.  Only for testing purposes.
	class NewDeleteArena
	{
	public:
		template<typename T = void>
		T* Allocate(uint64 count = 1)
		{
			return (T*)::operator new (sizeof(T) * count);
		}

		template<>
		void* Allocate<void>(uint64 size)
		{
			return ::operator new (size);
		}

		bool Deallocate(void* item)
		{
			delete item;
			return true;
		}
	};
}
#endif //HANDMADE_ALLOW_NEW

#endif //HANDMADE_MEMORY_NEWDELETEARENA_H