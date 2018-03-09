#ifndef HANDMADE_RECORDARENA_H
#define HANDMADE_RECORDARENA_H

#include "..\..\handmade_typedefs.h"
#include "..\..\handmade_funcdefs.h"
#include "..\Assert.h"

namespace Memory
{
	/** Allows you to resize the latest
	*/
	struct RecordArena
	{
		uint8 *buffer = nullptr;
		uint8 *bufferNext = nullptr;
		uint8 *bufferEnd = nullptr;
		uint64 size = 0;
		DeallocMemoryFunc *deallocateFunc = nullptr;
		bool ownsMemory = false;

		RecordArena(uint8 *bufferStart, uint64 bufferSize)
			: buffer(bufferStart), bufferNext(bufferStart), bufferEnd(bufferStart + bufferSize),
			  size(bufferSize), ownsMemory(false)
		{
			Assert(size >= sizeof(uint32));
			Assert(bufferStart != nullptr);
		}

		RecordArena(uint64 bufferSize, AllocMemoryFunc *allocator, DeallocMemoryFunc *deallocator)
		{
			buffer = allocator(bufferSize);
			if (buffer)
			{
				bufferNext = buffer;
				deallocateFunc = deallocator;
				bufferEnd = buffer + bufferSize;
				size = bufferSize;
				ownsMemory = true;
			}
		}

		/** Creates a StackArena at the start of the given block of memory, and tells it to manage the remaining memory in the block.
		*/
		static RecordArena *CreateContainedBlock(uint8 *blockStart, uint64 blockSize)
		{
			Assert(blockSize >= sizeof(RecordArena));
			*(RecordArena*)blockStart = RecordArena(blockStart + sizeof(RecordArena), blockSize - sizeof(RecordArena));
			return (RecordArena*)blockStart;
		}

		/** Allocates a new chunk of space (measured in bytes) from this arena.
		*/
		void *Allocate(uint32 allocSize)
		{
			if (bufferNext + allocSize + sizeof(uint32) > bufferEnd)
			{
				return nullptr;
			}
			*((uint32 *)bufferNext) = allocSize;
			bufferNext += sizeof(uint32);
			uint8 *result = bufferNext;
			bufferNext += allocSize;

			return result;
		}

		/** Deallocates starting at a certain point.
			Functionally the same as deallocate last, but here for consistency with other arenas.
		*/
		bool Deallocate(uint8 *item)
		{
			Assert(item >= buffer + sizeof(uint32));
			Assert(item < bufferEnd);

			uint8 *prevItem = bufferNext - *((uint32*)bufferNext + 1);
			//Bad pop - The item you're trying to remove is not the most recently allocated item!
			Assert(item = prevItem);

			bufferNext = item;
		}

		/** Pops last stack item.
		*/
		bool DeallocateLast()
		{
			if (bufferNext == buffer)
			{
				return false;
			}
			uint32 rollbackDist = *((uint32*)bufferNext - 1);
			Assert(bufferNext - rollbackDist >= buffer);

			bufferNext -= rollbackDist + sizeof(uint32);
			//CONSIDER(Ian):  Debug functionality that fills deallocated areas with junk data to ensure issues on access

			return true;
		}

		~StackArena()
		{
			if (ownsMemory)
			{
				deallocateFunc(buffer, size);
			}
			buffer = nullptr;
			bufferEnd = nullptr;
			bufferNext = nullptr;
			size = 0;
			ownsMemory = false;
			deallocateFunc = nullptr;
		}
	};
}

#endif // !HANDMADE_STACKARENA_H