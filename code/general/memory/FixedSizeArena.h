#ifndef HANDMADE_MEMORY_FIXEDSIZEARENA_H
#define HANDMADE_MEMORY_FIXEDSIZEARENA_H

#include "..\..\handmade_typedefs.h"
#include "..\..\handmade_funcdefs.h"
#include "..\Assert.h"

namespace Memory
{
	template <class Allocator = void>
	///An allocator with a fixed allocation size, set at creation.
	class FixedSizeArena
	{
	public:
		uint8 *buffer = nullptr;
		uint8 *bufferNext = nullptr;
		uint8 *bufferEnd = nullptr;
		uint8 **deletedListEnd = nullptr;
		Allocator *memoryAllocator = nullptr;
		uint64 size = 0;
		uint64 binSize = 0;
		bool ownsMemory = false;
		
		FixedSizeArena(uint8 *bufferStart, uint64 bufferSize, uint64 itemSize)
			: buffer(bufferStart), bufferNext(bufferStart), bufferEnd(bufferStart + bufferSize),
			size(bufferSize), binSize(itemSize), ownsMemory(false), deletedListEnd((uint8**)bufferEnd)
		{
			Assert(bufferStart != nullptr);
			Assert(itemSize > 0);
			Assert(bufferSize >= itemSize && bufferSize >= sizeof(uint8 *));
		}

		FixedSizeArena(uint64 bufferSize, uint64 itemSize, Allocator *allocator)
		{
			Assert(allocator != nullptr);
			Assert(itemSize > 0);
			Assert(bufferSize >= itemSize && bufferSize >= sizeof(uint8 *));
			buffer = (uint8 *)allocator->Allocate(bufferSize);
			Assert(buffer != nullptr);
			if (buffer)
			{
				bufferNext = buffer;
				bufferEnd = buffer + bufferSize;
				deletedListEnd = (uint8**)bufferEnd;
				memoryAllocator = allocator;
				size = bufferSize;
				binSize = itemSize;
				ownsMemory = true;
			}
		}

		///Swaps two instances.
		void Swap(FixedSizeArena &other)
		{
			uint8 *tempPtr = buffer;
			buffer = other.buffer;
			other.buffer = tempPtr;
			tempPtr = bufferNext;
			bufferNext = other.bufferNext;
			other.bufferNext = tempPtr;
			tempPtr = bufferEnd;
			bufferEnd = other.bufferEnd;
			other.bufferEnd = tempPtr;

			uint8 **tempPtrPtr = deletedListEnd;
			deletedListEnd = other.deletedListEnd;
			other.deletedListEnd = tempPtrPtr;

			Allocator *tempAlloc = memoryAllocator;
			memoryAllocator = other.memoryAllocator;
			other.memoryAllocator = tempAlloc;

			uint64 tempInt = binSize;
			binSize = other.binSize;
			other.binSize = tempInt;
			tempInt = size;
			size = other.size;
			other.size = tempInt;

			bool tempBool = ownsMemory;
			ownsMemory = other.ownsMemory;
			other.ownsMemory = tempBool;
		}

		FixedSizeArena(FixedSizeArena&& other)
		{
			Swap(other);
		}
		FixedSizeArena& FixedSizeArena::operator=(FixedSizeArena&& arg)
		{
			Swap(arg);
			return *this;
		}

		//FixedSizeArena(const FixedSizeArena&) = delete;
		//FixedSizeArena& FixedSizeArena::operator=(const FixedSizeArena& arg) = delete;

		/// Creates a FixedSizeArena at the start of the given block of memory, and tells it to manage the remaining memory in the block.
		static FixedSizeArena *CreateContainedBlock(uint8 *blockStart, uint64 blockSize, uint64 itemSize)
		{
			//TODO(Ian): make this optionally enforce a good alignment
			Assert(blockSize >= sizeof(FixedSizeArena) + sizeof(uint64));
			FixedSizeArena *resultArena = new (blockStart) FixedSizeArena(blockStart + sizeof(FixedSizeArena), blockSize - sizeof(FixedSizeArena), itemSize);
			return resultArena;
		}

		/// Returns the number of allocations left in the memory buffer.
		uint64 GetRemainingSlots() const
		{
			return (((uint64)deletedListEnd - (uint64)bufferNext) / binSize) + (((uint64)bufferEnd - (uint64)deletedListEnd) / sizeof(uint8 *));
		}

		///Returns a pointer to an open memory address.
		void *Allocate()
		{
			uint8 *result;

			if (deletedListEnd != (uint8**)bufferEnd)
			{
				result = *deletedListEnd;
				deletedListEnd++;
			}
			else if(bufferNext + binSize < bufferEnd)
			{
				result = bufferNext;
				bufferNext += binSize;
			}
			else
			{
				return nullptr;
			}

			return result;
		}

		///Returns a pointer to an open memory address.  Functionally the same as Allocate(), included to keep similarity to other allocators.
		void *Allocate(uint32 allocSize)
		{
			Assert(allocSize <= binSize);
			return Allocate();
		}

		///Frees the item at the given address.
		bool Deallocate(void *item)
		{
			if (((uint8 *)item < buffer || item >= bufferEnd)
				|| ((uint8 *)item - buffer) % binSize != 0)
			{
				//TODO(Ian):  Log bad deallocations
				return false;
			}

			if (item == bufferNext - binSize)
			{
				bufferNext -= binSize;
			}
			else
			{
				Assert((uint64)(deletedListEnd - 1) >= (uint64)bufferNext + binSize); //Deleted list collided with data!

				deletedListEnd--;
				*deletedListEnd = (uint8*)item;
			}
			return true;
		}

		~FixedSizeArena()
		{
			if (ownsMemory && buffer)
			{
				memoryAllocator->Deallocate(buffer);
			}
			buffer = nullptr;
			bufferEnd = nullptr;
			bufferNext = nullptr;
			deletedListEnd = nullptr;
			size = 0;
			binSize = 0;
			ownsMemory = false;
			memoryAllocator = nullptr;
		}
	};

	template<>
	FixedSizeArena<void>::~FixedSizeArena()
	{
		buffer = nullptr;
		bufferEnd = nullptr;
		bufferNext = nullptr;
		deletedListEnd = nullptr;
		size = 0;
		binSize = 0;
		ownsMemory = false;
		memoryAllocator = nullptr;
	}
}

#endif // !HANDMADE_FIXEDSIZEARENA_H