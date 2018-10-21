#ifndef HANDMADE_HMSTRING_H
#define HANDMADE_HMSTRING_H

#include "../handmade_typedefs.h"

class HMString
{
	uint32 size;
	char *text;

public:
	HMString(char* text, uint32 size) : size(size), text(text) {}

	uint32 Length() { return size; }
	char *RawCString() { return text; }
	char operator [] (uint32 index) { return text[index]; }
};

#endif