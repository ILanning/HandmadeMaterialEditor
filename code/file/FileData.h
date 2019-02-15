#ifndef HANDMADE_FILEDATA_H
#define HANDMADE_FILEDATA_H

#include "..\handmade_typedefs.h"

///A simple stuct containing a file and its most basic information.
struct FileData
{
	///Whether or not the file at the given path has been loaded into the File member.
	bool IsLoaded;
	///The size of the path.
	int32 PathSize;
	///The path to the file.
	char* Path;
	///The size of the file.
	uint32 FileSize;
	///The raw data from the file.
	void *File;

	/**
	\brief Loads the contents of the file into *File if they have not already been loaded.

	\readFile The function to use to load this file.
	\reload Set to true to force this file to be loaded fresh
	*/
};

#endif // !HANDMADE_FILEDATA_H
