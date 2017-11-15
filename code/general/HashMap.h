#ifndef HANDMADE_HASHMAP
#define HANDMADE_HASHMAP

#include "../handmade_typedefs.h"
#include "../drawing/Vertex.h"

struct HashNode
{
	int32 Key;
	VertexNormalTexture *Value;

private:
	HashNode *Next;
};

struct HashMap
{
	//Arranged as array of linked lists
private:
	HashNode *[]buckets;
	int32 bucketCount;
	int32 occupied;


	HashMap(int32 capacity)
	{
		bucketCount = capacity;

	}


};

//template KeyValuePair <K key, V value>

#endif