#ifndef HANDMADE_HASHMAP_H
#define HANDMADE_HASHMAP_H

#include "../handmade_typedefs.h"
#include "HMPair.h"
#include "HMString.h"
#include "StaticArray.h"
#define XXH_PRIVATE_API
#include "../libraries/xxhash.h"

namespace Collections
{
	inline uint64 GetHash(HMString* data)
	{
		return XXH64(data->RawCString(), data->Length(), 0);
	}

	inline uint64 GetHash(HMString data)
	{
		return XXH64(data.RawCString(), data.Length(), 0);
	}

	inline uint64 GetHash(int32 data)
	{
		return XXH64(&data, 4, 0);
	}
	// TODO: Set up KeyType as something that must be Hashable

	//A dictionary/unordered map data structure.
	template <class KeyType, class ValueType, class Allocator>
	class HashMap
	{
		//Arranged as flat array that fills buckets in sequence if desired bucket is full
		//TODO: Alternate version with three (probably contiguous) arrays, one for each member
		struct HashItem
		{
			KeyType Key;
			ValueType Value;
			int32 HashCode;
		};

		int32 maxAlreadyFilled = 8;
		real32 upsizeFactor = 2.0f;
		static const int32 baseSize = 4;

		Allocator *memory = nullptr;
		HashItem *buckets = nullptr;
		int32 bucketCount = 0;
		int32 occupied = 0;

		void swap(HashMap& other)
		{
			real32 tempFloat = other.upsizeFactor;
			other.upsizeFactor = upsizeFactor;
			upsizeFactor = tempFloat;

			Allocator* tempMem = other.memory;
			other.memory = memory;
			memory = tempMem;

			HashItem* tempItems = other.buckets;
			other.buckets = buckets;
			buckets = tempItems;

			int32 tempInt = other.bucketCount;
			other.bucketCount = bucketCount;
			bucketCount = tempInt;

			tempInt = other.occupied;
			other.occupied = occupied;
			occupied = tempInt;

			tempInt = other.maxAlreadyFilled;
			other.maxAlreadyFilled = maxAlreadyFilled;
			maxAlreadyFilled = tempInt;
		}

	public:
		HashMap() {}

		HashMap(Allocator *allocator) : bucketCount(baseSize), memory(allocator)
		{
			buckets = memory->Allocate<HashItem>(bucketCount);

			for (int32 i = 0; i < bucketCount; i++)
			{
				buckets[i].HashCode = -1;
			}
		}

		HashMap(int32 capacity, Allocator *allocator) : bucketCount(capacity), memory(allocator)
		{
			buckets = memory->Allocate<HashItem>(bucketCount);

			for (int32 i = 0; i < bucketCount; i++)
			{
				buckets[i].HashCode = -1;
			}
		}

		HashMap(StaticArray<HMPair<KeyType, ValueType>> pairs, Allocator *allocator) : memory(allocator)
		{
			real32 newCount = baseSize;
			while (newCount < (int32)pairs.Length)
			{
				newCount *= upsizeFactor;
			}
			bucketCount = (int32)newCount;
			buckets = memory->Allocate<HashItem>(bucketCount);

			for (int32 i = 0; i < bucketCount; i++)
			{
				buckets[i].HashCode = -1;
			}

			AddMany(pairs);
		}

		HashMap(HashMap& other)
		{
			memory = other.memory;
			bucketCount = other.bucketCount;
			occupied = other.occupied;
			buckets = memory->Allocate<HashItem>(bucketCount);

			for (int32 i = 0; i < bucketCount; i++)
			{
				buckets[i] = other.buckets[i];
			}
		}

		HashMap(HashMap&& other) : HashMap()
		{
			swap(other);
		}

		HashMap& operator=(HashMap other)
		{
			swap(other);
			return *this;
		}

		ValueType& operator [] (const KeyType& key) 
		{
			int32 bucket = internalFind(hashTransform(key));
			if (bucket == -1)
			{
				return internalAdd(key);
			}
			else
			{
				return buckets[bucket].Value;
			}
		}

		// Adds a new key/value pair to the HashMap, or overwrites the old value if the key already exists.
		void Add(KeyType key, ValueType value)
		{
			int32 bucket = internalFind(hashTransform(key));
			if (bucket == -1)
			{
				ValueType& valueLocation = internalAdd(key);
				valueLocation = value;
			}
			else
			{
				buckets[bucket].Value = value;
			}
		}

		// Adds a set of new key/value pairs to the HashMap, overwriting old values if a key already exists.
		void AddMany(StaticArray<HMPair<KeyType, ValueType>>& pairs)
		{
			for (uint32 i = 0; i < pairs.Length; i++)
			{
				Add(pairs[i].First, pairs[i].Second);
			}
		}

		// Merges the data from another HashMap into this one.  Prefers to keep the argument's value in cases where both contain the same key.
		void Combine(HashMap& other)
		{
			for (int32 i = 0; i < other.bucketCount; i++)
			{
				if (other.buckets[i].HashCode > 0)
				{
					Add(other.buckets[i].Key, other.buckets[i].Value);
				}
			}
		}

		// Returns a pointer to the value of the given key, or null if the key does not exist.  This pointer does not belong to the caller.
		ValueType* GetAt(const KeyType& key) const
		{
			int32 item = internalFind(hashTransform(key));

			if (item != -1)
			{
				return &(buckets[item].Value);
			}

			return nullptr;
		}

		// Returns true if the given key exists in the Hashmap, and false otherwise.
		bool CheckExists(const KeyType& key) const
		{
			return internalFind(hashTransform(key)) != -1;
		}

		//Removes the given key/value from the HashMap if the given key exists.
		void Remove(const KeyType& key)
		{
			int32 item = internalFind(hashTransform(key));

			if (item != -1)
			{
				buckets[item].HashCode = -1;
				occupied--;
				// TODO: What happens to allocated keys?
				buckets[item].Value = ValueType();
			}
		}

		template<class ArrayAllocator>
		StaticArray<HMPair<KeyType, ValueType>> ToArray(ArrayAllocator alloc)
		{
			auto resultArray = alloc.Allocate<HMPair<KeyType, ValueType>>(occupied);

			int32 resultIter = 0;
			for (int32 i = 0; i < bucketCount; i++)
			{
				if (buckets[i].HashCode > 0)
				{
					resultArray[resultIter].First = buckets[i].Key;
					resultArray[resultIter].Second = buckets[i].Value;
					i++;
				}
			}
			return { resultArray, occupied };
		}

		// Returns the number of items in the HashMap.
		int32 Length() { return occupied; }
		// Returns the number of remaining spaces in the HashMap.
		int32 Capacity() { return bucketCount; }

		~HashMap()
		{
			if (memory && buckets)
			{
				memory->Deallocate(buckets);
				buckets = nullptr;
				memory = nullptr;
			}

			bucketCount = 0;
			occupied = 0;
		}

	private:
		int32 hashTransform(const KeyType key) const
		{
			// Clip off the top bit for use as a delete flag, then shift down
			return (int32)(GetHash(key) << 1 >> 33);
		}

		void upsizeArray(int32 newCapacity, int32 extraHash)
		{
			// TODO: Test doubling size on shuffle fail vs just adding a single space
			// Also test making the array slightly larger than it needs to be so we can reroll it without reallocating
			
			HashItem *newArray = nullptr;
			bool fitAllItems = false;
			if (newCapacity < baseSize)
			{
				newCapacity = baseSize;
			}

			while (!fitAllItems)
			{
				fitAllItems = true;
				newArray = memory->Allocate<HashItem>(newCapacity);

				for (int32 newIter = 0; newIter < newCapacity; newIter++)
				{
					newArray[newIter].HashCode = -1;
				}

				int32 extraHashBucket = checkAdd(extraHash, newArray, newCapacity);
				newArray[extraHashBucket].HashCode = extraHash;

				for (int32 i = 0; i < bucketCount; i++)
				{
					int32 newBucket = checkAdd(buckets[i].HashCode, newArray, newCapacity);

					if (newBucket == -1)
					{
						fitAllItems = false;
						break;
					}

					newArray[newBucket].HashCode = buckets[i].HashCode;
				}

				if (!fitAllItems)
				{
					memory->Deallocate(newArray);
					newCapacity = (int32)(newCapacity * upsizeFactor);
					continue;
				}
			}

			for (int32 i = 0; i < bucketCount; i++)
			{
				int32 newBucket = checkAdd(buckets[i].HashCode, newArray, newCapacity);
				newArray[newBucket].Key = buckets[i].Key;
				newArray[newBucket].Value = buckets[i].Value;
			}

			memory->Deallocate(buckets);
			buckets = newArray;
			bucketCount = newCapacity;
		}

		// Returns a bucket index if there's space for the item or if the item already exists, and -1 otherwise.
		int32 checkAdd(int32 hash, const HashItem *data, int32 size) const
		{
			int32 foundBucket = -1;
			int32 modHash = hash % size;

			for (int i = modHash; i - modHash < maxAlreadyFilled; i++)
			{
				if (hash == data[i % size].HashCode)
				{
					foundBucket = i % size;
					break;
				}
				else if (foundBucket == -1 && data[i % size].HashCode == -1)
				{
					foundBucket = i % size;
				}
			}

			return foundBucket;
		}

		ValueType& internalAdd(KeyType key)
		{
			int32 hash = hashTransform(key);
			int32 item = checkAdd(hash, buckets, bucketCount);

			if (item == -1)
			{
				upsizeArray((int32)(bucketCount * upsizeFactor), hash);
				item = checkAdd(hash, buckets, bucketCount);
			}

			buckets[item].Key = key;
			buckets[item].HashCode = hash;
			occupied++;

			return buckets[item].Value;
		}

		// Returns a bucket index if the item is in the array, and -1 if not.
		int32 internalFind(int32 hash) const
		{
			int32 modHash = hash % bucketCount;
			for (int i = modHash; i - modHash < maxAlreadyFilled; i++)
			{
				if (hash == buckets[i % bucketCount].HashCode)
				{
					return i % bucketCount;
				}
			}

			return -1;
		}
	};
}

#endif //HANDMADE_HASHMAP_H