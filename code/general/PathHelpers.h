#ifndef HANDMADE_PATHHELPERS_H
#define HANDMADE_PATHHELPERS_H

#include "../handmade_typedefs.h"

namespace Path
{
	bool IsAbsolute(const char *string);
	bool IsRelative(const char *string);

	inline bool IsPathSeperator(char character)
	{
		//TODO: Also handle "\\" and "//"
		return character == '\\' || character == '/';
	}

	char *Combine(const char *a, const char* b, int32 aOffset = 0, int32 bOffset = 0, int32 *outLength = nullptr);

	/**
	\brief Returns a copy of the given path containing everything but the final element.
	*/
	char *CloneParentDirectory(const char *path, int32 *outFinalLength = nullptr);

	/**
	\brief Returns a copy of the name of the directory or file at the end of the path chain (including extension)
	*/
	char *CloneEndName(const char *path, int32 *outFinalLength = nullptr);
}

#endif //HANDMADE_PATHHELPERS_H