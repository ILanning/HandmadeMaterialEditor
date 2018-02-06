#ifndef HANDMADE_FIXEDSIZEARENA_H
#define HANDMADE_FIXEDSIZEARENA_H

#include "..\..\handmade_typedefs.h"
#include "..\..\handmade_funcdefs.h"
#include "..\Assert.h"

namespace Memory
{
	class FixedSizeArena
	{
		uint8 *buffer = nullptr;
		uint8 *bufferNext = nullptr;
		uint8 *bufferEnd = nullptr;
		uint64 size = 0;
		uint64 binSize = 0;
		uint8 **deletedListEnd = nullptr;
		DeallocMemoryFunc *deallocateFunc = nullptr;
		bool ownsMemory = false;
		
	public:
		FixedSizeArena(uint8 *bufferStart, uint64 bufferSize, uint64 itemSize)
			: buffer(bufferStart), bufferNext(bufferStart), bufferEnd(bufferStart + bufferSize),
			size(bufferSize), binSize(itemSize), ownsMemory(false), deletedListEnd(bufferEnd)
		{
			Assert(memoryFront != nullptr);
			Assert(itemSize > 0);
			Assert(byteSize > itemSize);
		}

		/** Creates a StackArena at the start of the given block of memory, and tells it to manage the remaining memory in the block.
		*/
		static FixedSizeArena *CreateContainedBlock(uint8 *blockStart, uint64 blockSize, uint64 itemSize)
		{
			Assert(blockSize >= sizeof(FixedSizeArena));
			*(FixedSizeArena*)blockStart = FixedSizeArena(blockStart + sizeof(FixedSizeArena), blockSize - sizeof(FixedSizeArena), itemSize);
			return (FixedSizeArena*)blockStart;
		}

		void *Allocate()
		{
			Assert(bufferNext + binSize < deletedListEnd);
			uint8 *result = nullptr;
			if (deletedListEnd != bufferEnd)
			{
				result = *deletedListEnd;
				deletedListEnd++;
				return result;
			}
		}

		void *Allocate(uint32 allocSize)
		{
			Assert(allocSize == binSize);
			return Allocate();
		}

		/*void *AllocateMany(uint64 count)
		{

		}*/

		bool Deallocate(void *item)
		{
			deletedListEnd--;
			Assert(deletedListEnd >= bufferNext + binSize);
			*deletedListEnd = (uint8*)item;
		}

		/*bool DeallocateMany(void *start, uint64 count)
		{
			Assert(start + count);
			for (int32 i = 0; i < count; i++)
			{
				Deallocate(start + i * binSize);
			}
		}*/
	};
}

#endif // !HANDMADE_FIXEDSIZEARENA_H