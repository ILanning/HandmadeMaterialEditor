#ifndef HANDMADE_FUNCDEFS_H
#define HANDMADE_FUNCDEFS_H

#include "handmade_typedefs.h"
#include "PlatformGameSettings.h"
#include "file\FileData.h"

#define PLATFORM_ALLOC_MEMORY(name) uint8 *name(uint64 size, bool *outSuccess)
typedef PLATFORM_ALLOC_MEMORY(AllocMemoryFunc);

#define PLATFORM_DEALLOC_MEMORY(name) void name(uint8 *memory, uint64 size, bool *outSuccess)
typedef PLATFORM_DEALLOC_MEMORY(DeallocMemoryFunc);

#define PLATFORM_UPDATE_SETTINGS(name) void name(PlatformGameSettings settings)
typedef PLATFORM_UPDATE_SETTINGS(PlatformUpdateSettingsFunc);

#define PLATFORM_READ_FILE(name) FileData name(char *path, int32 pathLength, bool *outSuccess)
typedef PLATFORM_READ_FILE(ReadFileFunc);

#endif // !HANDMADE_FUNCDEFS_H
