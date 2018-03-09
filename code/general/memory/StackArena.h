#ifndef HANDMADE_STACKARENA_H
#define HANDMADE_STACKARENA_H

#include "..\..\handmade_typedefs.h"
#include "..\..\handmade_funcdefs.h"
#include "..\Assert.h"

namespace Memory
{
	struct StackArena
	{
		uint8 *buffer = nullptr;
		uint8 *bufferNext = nullptr;
		uint8 *bufferEnd = nullptr;
		uint32 upcomingSize = 0;
		bool partialInProgress = false;
		uint64 size = 0;
		DeallocMemoryFunc *deallocateFunc = nullptr;
		bool ownsMemory = false;

		StackArena(uint8 *bufferStart, uint64 bufferSize)
			: buffer(bufferStart), bufferNext(bufferStart), bufferEnd(bufferStart + bufferSize),
			  size(bufferSize), ownsMemory(false)
		{
			Assert(size >= sizeof(uint32));
			Assert(bufferStart != nullptr);
		}

		StackArena(uint64 bufferSize, AllocMemoryFunc *allocator, DeallocMemoryFunc *deallocator)
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
		static StackArena *CreateContainedBlock(uint8 *blockStart, uint64 blockSize)
		{
			Assert(blockSize >= sizeof(StackArena));
			*(StackArena*)blockStart = StackArena(blockStart + sizeof(StackArena), blockSize - sizeof(StackArena));
			return (StackArena*)blockStart;
		}

		//TODO(Ian): GetRemaining

		/** Allocates a new chunk of space (measured in bytes) from this arena.
		*/
		void *Allocate(uint32 allocSize)
		{
			Assert(partialInProgress == false); //Close partial allocation before starting a new allocation!
			if (bufferNext + allocSize + sizeof(uint32) > bufferEnd)
			{
				return nullptr;
			}
			uint8 *result = bufferNext;
			bufferNext += allocSize;
			*((uint32 *)bufferNext) = allocSize;
			bufferNext += sizeof(uint32);

			return result;
		}

		/** Creates a new open ended allocation, or expands the existing one.  Returns a pointer to the start of the new memory chunk if successful, or null on failure.
		*/
		void *PartialAlloc(uint32 allocSize)
		{
			if (!partialInProgress)
			{
				if (bufferNext + allocSize + sizeof(uint32) > bufferEnd)
				{
					return nullptr;
				}
				partialInProgress = true;
				upcomingSize = allocSize;
			}
			else
			{
				if (bufferNext + upcomingSize + allocSize + sizeof(uint32) > bufferEnd)
				{
					return nullptr;
				}
				upcomingSize += allocSize;
			}
			bufferNext += allocSize;
			return bufferNext;
		}

		uint32 GetPartialAllocCurrentSize()
		{
			return upcomingSize;
		}

		/** Closes out a partial allocation, allowing new allocations and deallocations.
		*/
		void ClosePartialAlloc()
		{
			*((uint32 *)bufferNext) = upcomingSize;
			bufferNext += sizeof(uint32);
			partialInProgress = false;
		}

		/** Reduces the size of a partial allocation by a certain amount.  Returns the location of the new allocation end.
		*/
		void *ReducePartialAllocBy(uint32 deallocAmount)
		{
			Assert(deallocAmount <= upcomingSize);

			upcomingSize -= deallocAmount;
			bufferNext -= deallocAmount;
			return bufferNext;
		}

		/** Reduces a partial allocation to a specific size.  Returns the location of the new allocation end.
		*/
		void *ReducePartialAllocTo(uint32 newSize)
		{
			Assert(newSize <= upcomingSize);

			bufferNext -= upComingSize - newSize;
			upcomingSize = newSize;
			return bufferNext;
		}

		/** Deallocates starting at a certain point.
			Functionally the same as deallocate last, but here for consistency with other arenas.
		*/
		bool Deallocate(uint8 *item)
		{
			Assert(partialInProgress == false);
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
			Assert(partialInProgress == false);
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