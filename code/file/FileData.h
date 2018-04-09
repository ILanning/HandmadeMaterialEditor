#ifndef HANDMADE_FILEDATA_H
#define HANDMADE_FILEDATA_H

#include "..\handmade_typedefs.h"

struct FileData
{
	bool IsLoaded;
	int32 PathSize;
	char* Path;
	uint32 FileSize;
	void *File;

	/**
	\brief Loads the contents of the file into *File if they have not already been loaded.

	\readFile The function to use to load this file.
	\reload Set to true to force this file to be loaded fresh
	*/
};

#endif // !HANDMADE_FILEDATA_H
