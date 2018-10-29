#ifndef HANDMADE_HMSTRING_H
#define HANDMADE_HMSTRING_H

#include "../handmade_typedefs.h"
#include "StringHelpers.cpp"

// Contains a character array and a size.  Must end in a null, but may also contain them in the middle of the text/
class HMString
{
	uint32 size = 0;
	char* text = nullptr;

public:
	HMString() {}
	HMString(char* text) : text(text)
	{
		int32 i = 0;
		while (text[i] != '\0')
		{
			i++;
		}
		size = i + 1;
	}
	HMString(char* text, uint32 size) : size(size), text(text) {}

	HMString(const HMString &other)
	{
		text = CString::CopySubstring(other.RawCString(), other.Length() - 1);
		size = other.Length();
	}

	uint32 Length() const { return size; }
	char* RawCString() { return text; }
	char* RawCString() const { return text; }
	char& operator [] (uint32 index) { return text[index]; }
	char operator [] (uint32 index) const { return text[index]; }

	int32 CopyTo(char* target, uint32 targetLength, int32 offset)
	{
		uint32 i = 0;
		for (; i < size - 1 && i + offset < targetLength; i++)
		{
			target[i + offset] = text[i];
		}
		return i + offset;
	}

	bool operator ==(const HMString& other) const
	{
		if (this == &other)
		{
			return true;
		}

		if (size != other.size)
		{
			return false;
		}

		if (text == other.text)
		{
			return true;
		}

		for (uint32 i = 0; i < size; i++)
		{
			if (text[i] != other.text[i])
			{
				return false;
			}
		}

		return true;
	}

	bool operator !=(const HMString& other) const
	{
		return !(*this == other);
	}

	bool operator ==(const char* other) const
	{
		if (text == other)
		{
			return true;
		}

		for (uint32 i = 0; i < size; i++)
		{
			if (text[i] != other[i])
			{
				return false;
			}
		}

		return true;
	}

	bool operator !=(const char* other) const
	{
		return !(*this == other);
	}
};

#ifdef WIN32_HANDMADE_TEST

std::ostream& operator << (std::ostream& os, const HMString& string)
{
	os << "(\"" << string.RawCString() << "\", " << string.Length() << ')';
	return os;
}

#endif

#endif