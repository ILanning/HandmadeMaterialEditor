#ifndef HANDMADE_FUNCDEFS_H

#include "handmade_typedefs.h"
#include "PlatformGameSettings.h"

#define PLATFORM_ALLOC_MEMORY(name) uint8 *name(uint64 size, bool outSuccess)
typedef PLATFORM_ALLOC_MEMORY(AllocMemoryFunc);

#define PLATFORM_DEALLOC_MEMORY(name) void name(uint8 *memory, uint64 size, bool outSuccess)
typedef PLATFORM_DEALLOC_MEMORY(DeallocMemoryFunc);

#define PLATFORM_UPDATE_SETTINGS(name) void name(PlatformGameSettings settings)
typedef PLATFORM_UPDATE_SETTINGS(PlatformUpdateSettingsFunc);

#endif // !HANDMADE_FUNCDEFS_H
