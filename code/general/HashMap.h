#ifndef HANDMADE_HASHMAP_H
#define HANDMADE_HASHMAP_H

#include "../handmade_typedefs.h"

template<class KeyType, class ValueType>
class HashMap
{
	//Arranged as array of linked lists
	struct HashNode
	{
		KeyType Key;
		ValueType Value;
		HashNode *Next;
	};

	HashNode *buckets;
	int32 bucketCount;
	int32 occupied;

public:
	HashMap(int32 capacity)
	{
		bucketCount = capacity;

	}


};

//template KeyValuePair <K key, V value>

#endif //HANDMADE_HASHMAP_H