#ifndef HANDMADE_FUNCDEFS_H
#define HANDMADE_FUNCDEFS_H

#include "handmade_typedefs.h"
#include "file\FileData.h"

// Platform interaction functions

#define PLATFORM_ALLOC_MEMORY(name) uint8* name(uint64 size, bool* outSuccess)
typedef PLATFORM_ALLOC_MEMORY(PlatformAllocMemoryFunc);

#define PLATFORM_DEALLOC_MEMORY(name) void name(uint8* memory, uint64 size, bool* outSuccess)
typedef PLATFORM_DEALLOC_MEMORY(PlatformDeallocMemoryFunc);

#define PLATFORM_READ_FILE(name) FileData name(char* path, int32 pathLength, bool* outSuccess)
typedef PLATFORM_READ_FILE(ReadFileFunc);

#define PLATFORM_WRITE_FILE(name) void name(char* path, int32 pathLength, void* file, int32 fileLength, bool* outSuccess)
typedef PLATFORM_WRITE_FILE(WriteFileFunc);


// General game usage functions

#define GENERAL_ALLOC_MEMORY(name) uint8* name(void* allocator, uint64 size, bool* outSuccess)
typedef GENERAL_ALLOC_MEMORY(AllocMemoryFunc);

#define GENERAL_DEALLOC_MEMORY(name) void name(void* allocator, uint8 *memory, uint64 size, bool* outSuccess)
typedef GENERAL_DEALLOC_MEMORY(DeallocMemoryFunc);

#endif // !HANDMADE_FUNCDEFS_H
