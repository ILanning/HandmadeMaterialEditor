#ifndef HANDMADE_DEBUGFUNCS_H
#define HANDMADE_DEBUGFUNCS_H

#include "handmade_typedefs.h"
#include "ThreadContext.h"

//NOTE(casey): Services that the platform layer provides to the game
#define HANDMADE_INTERNAL 1
#if HANDMADE_INTERNAL
/* IMPORTANT(casey):

   These are NOT for doing anything in the shipping game - they are
   blocking and the write doesn't protect against lost data!
*/
struct debug_read_file_result
{
	uint32 ContentsSize;
	void *Contents;
};

#define DEBUG_PLATFORM_FREE_FILE_MEMORY(name) void name(ThreadContext *Thread, void *Memory)
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY(debug_platform_free_file_memory);

#define DEBUG_PLATFORM_READ_ENTIRE_FILE(name) debug_read_file_result name(ThreadContext *Thread, const char *Filename, bool *outSuccess)
typedef DEBUG_PLATFORM_READ_ENTIRE_FILE(debug_platform_read_entire_file);

#define DEBUG_PLATFORM_WRITE_ENTIRE_FILE(name) bool32 name(ThreadContext *Thread, const char *Filename, uint32 MemorySize, const void *Memory)
typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE(debug_platform_write_entire_file);

#define DEBUG_PLATFORM_MESSAGE_ERROR_FUNC(name) void name(const char *errorString)
typedef DEBUG_PLATFORM_MESSAGE_ERROR_FUNC(DebugMessageErrorFunc);

#endif //HANDMADE_DEBUGFUNCS_H

#endif