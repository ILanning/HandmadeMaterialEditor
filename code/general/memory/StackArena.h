 #ifndef HANDMADE_MEMORY_STACKARENA_H
#define HANDMADE_MEMORY_STACKARENA_H

#include "..\..\handmade_typedefs.h"
#include "..\..\handmade_funcdefs.h"
#include "..\Assert.h"

namespace Memory
{
	template<class Allocator = void>
	///A stack-based memory allocator.  Supports resizing the latest allocation without actually moving any data, assuming the allocator still has space.
	class StackArena
	{
	public:
		/// The beginning of the memory space controlled by this object.
		uint8 *buffer = nullptr;
		/// The next open space in the buffer.
		uint8 *bufferNext = nullptr;
		/// The last address that this object controls.
		uint8 *bufferEnd = nullptr;
		/// The allocator this object's memory came from.
		Allocator *memoryAllocator = nullptr;
		/// The size of the partial allocation currently in progress.
		uint64 upcomingSize = 0;
		/// The size of the buffer.
		uint64 size = 0;
		/// Whether or not the object owns its own memory.
		bool ownsMemory = false;
		/// Whether or not a partial allocation is in progress.
		bool partialInProgress = false;

		///Swaps two instances.
		void Swap(StackArena &other)
		{
			uint8 *tempBuffer = buffer;
			buffer = other.buffer;
			other.buffer = tempBuffer;
			tempBuffer = bufferNext;
			bufferNext = other.bufferNext;
			other.bufferNext = tempBuffer;
			tempBuffer = bufferEnd;
			bufferEnd = other.bufferEnd;
			other.bufferEnd = tempBuffer;

			uint64 tempInt = upcomingSize;
			upcomingSize = other.upcomingSize;
			other.upcomingSize = tempInt;
			tempInt = size;
			size = other.size;
			other.size = tempInt;

			bool tempBool = partialInProgress;
			partialInProgress = other.partialInProgress;
			other.partialInProgress = tempBool;
			tempBool = ownsMemory;
			ownsMemory = other.ownsMemory;
			other.ownsMemory = tempBool;

			Allocator *tempalloc = memoryAllocator;
			memoryAllocator = other.memoryAllocator;
			other.memoryAllocator = tempalloc;
		}

		StackArena(uint8 *bufferStart, uint64 bufferSize)
			: buffer(bufferStart), bufferNext(bufferStart), bufferEnd(bufferStart + bufferSize),
			  size(bufferSize), ownsMemory(false)
		{
			Assert(size >= sizeof(uint64));
			Assert(bufferStart != nullptr);
		}

		StackArena(Allocator *allocator, uint64 bufferSize)
		{
			buffer = (uint8 *)allocator->Allocate(bufferSize);
			Assert(buffer != nullptr);
			if (buffer)
			{
				bufferNext = buffer;
				memoryAllocator = allocator;
				bufferEnd = buffer + bufferSize;
				size = bufferSize;
				ownsMemory = true;
			}
		}

		StackArena(StackArena&& other)
		{
			Swap(other);
		}
		StackArena& StackArena::operator=(StackArena&& arg)
		{
			Swap(arg);
			return *this;
		}

		//StackArena(const StackArena&) = delete;
		//StackArena& StackArena::operator=(const StackArena& arg) = delete;

		/// Creates a StackArena at the start of the given block of memory, and tells it to manage the remaining memory in the block.
		static StackArena *CreateContainedBlock(uint8 *blockStart, uint64 blockSize)
		{
			//TODO(Ian): make this optionally enforce a good alignment
			Assert(blockSize >= sizeof(StackArena) + sizeof(uint64));
			StackArena *resultArena = new (blockStart) StackArena(blockStart + sizeof(StackArena), blockSize - sizeof(StackArena));
			return resultArena;
		}

		/// Returns the number of bytes left in the memory buffer, including those used up by an ongoing partial allocation.
		uint64 GetRemainingBytes() const
		{
			return (uint64)bufferEnd - ((uint64)bufferNext + upcomingSize);
		}

		/// Returns the number of bytes used by the current partial allocation (or zero if there isn't one going).
		uint64 GetPartialAllocCurrentSize() const
		{
			return upcomingSize;
		}

		template <typename T = void>
		/// Allocates (but does not initialize) a new set of items from this arena.
		T *Allocate(const uint64 count = 1)
		{
			return (T*)Allocate(sizeof(T) * count);
		}

		template <>
		/// Allocates a new chunk of space (measured in bytes) from this arena.
		void *Allocate<void>(const uint64 allocSize)
		{
			Assert(partialInProgress == false); //Close partial allocation before starting a new allocation!

			if (allocSize == 0 || allocSize + sizeof(uint64) > GetRemainingBytes())
			{
				return nullptr;
			}

			uint8 *result = bufferNext;
			bufferNext += allocSize;
			*((uint64 *)bufferNext) = allocSize;
			bufferNext += sizeof(uint64);

			return result;
		}

		/// Creates a new open ended allocation, or expands the existing one.  Returns a pointer to the start of the new memory chunk if successful, or null on failure.
		void *PartialAlloc(const uint64 allocSize)
		{
			if (allocSize == 0 || allocSize + sizeof(uint64) > GetRemainingBytes())
			{
				return nullptr;
			}

			if (!partialInProgress)
			{
				partialInProgress = true;
			}
			upcomingSize += allocSize;

			return bufferNext + upcomingSize - allocSize;
		}

		/// Closes out a partial allocation, allowing new allocations and deallocations.
		void ClosePartialAlloc()
		{
			bufferNext += upcomingSize;
			*((uint64 *)bufferNext) = upcomingSize;
			bufferNext += sizeof(uint64);
			partialInProgress = false;
			upcomingSize = 0;
		}

		///Cancels the currently open partial allocation, returning the allocator to its previous state.
		void CancelPartialAlloc()
		{
			partialInProgress = false;
			upcomingSize = 0;
		}

		/// Reduces the size of a partial allocation by a certain amount.  Returns the location of the new allocation end.
		void *ReducePartialAllocBy(uint64 deallocAmount)
		{
			Assert(deallocAmount <= upcomingSize && deallocAmount >= 0);

			upcomingSize -= deallocAmount;
			return bufferNext;
		}

		/// Reduces a partial allocation to a specific size.  Returns the location of the new allocation end.
		void *ReducePartialAllocTo(uint64 newSize)
		{
			Assert(newSize <= upcomingSize && newSize >= 0);

			upcomingSize = newSize;
			return bufferNext;
		}

		/// Pops the last stack item.
		bool DeallocateLast()
		{
			//CONSIDER(Ian): Should this fail if a partial is open, or cancel the partial?
			Assert(partialInProgress == false);
			if (bufferNext == buffer)
			{
				return false;
			}
			uint64 rollbackDist = *((uint64*)bufferNext - 1);
			Assert(bufferNext - rollbackDist >= buffer);

			bufferNext -= rollbackDist + sizeof(uint64);
			//CONSIDER(Ian):  Debug functionality that fills deallocated areas with junk data/null to encourage issues on access

			return true;
		}

		/// Deallocates starting at a certain point.  Functionally the same as deallocate last, but here for consistency with other arenas.
		bool Deallocate(void *item)
		{
			uint8 *prevItem = bufferNext - *((uint64 *)bufferNext - 1) - sizeof(uint64);
			if ((uint8 *)item != prevItem) //Bad pop - The item you're trying to remove is not the most recently allocated item!
			{
				return false;
			}

			return DeallocateLast();
		}

		~StackArena()
		{
			if (ownsMemory && buffer)
			{
				memoryAllocator->Deallocate(buffer);
			}
			buffer = nullptr;
			bufferEnd = nullptr;
			bufferNext = nullptr;
			size = 0;
			ownsMemory = false;
			memoryAllocator = nullptr;
		}
	};

	template<>
	StackArena<void>::~StackArena()
	{
		buffer = nullptr;
		bufferEnd = nullptr;
		bufferNext = nullptr;
		size = 0;
		ownsMemory = false;
		memoryAllocator = nullptr;
	}
}

#endif // !HANDMADE_MEMORY_STACKARENA_H