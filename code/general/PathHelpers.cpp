#ifndef HANDMADE_PATHHELPERS
#define HANDMADE_PATHHELPERS

#include "../handmade.h"
#include "StringHelpers.cpp"

namespace Path
{
	bool IsAbsolute(char *string)
	{
		if (!string) //CONSIDER(Ian): Do we want this to just crash if it's given a null pointer?
		{
			return false;
		}

		int32 start = 0;
		while (string[start] != '\0' && CString::IsWhitespace(string[start]))
		{
			start++;
		}
		return CString::IsLetter(string[start]) && string[start + 1] == ':';
	}

	bool IsRelative(char *string)
	{
		if (!string)
		{
			return false;
		}

		int32 start = 0;
		while (string[start] != '\0' && CString::IsWhitespace(string[start]))
		{
			start++;
		}
		return !(CString::IsLetter(string[start]) && string[start + 1] == ':');
	}

	bool IsPathSeperator(char character)
	{
		//TODO(Ian): Also handle "\\" and "//"
		return character == '\\' || character == '/';
	}

	char *Combine(char *a, char* b, int32 aOffset = 0, int32 bOffset = 0, int32 *outLength = nullptr)
	{
		Assert(IsRelative(b));
		Assert(a != nullptr);
		Assert(b != nullptr);

		int32 aLength = CString::GetLength(a);
		int32 bLength = CString::GetLength(b);
		
		//Null terminator from a becomes a path seperator
		int32 finalLength = aLength - aOffset + bLength - bOffset;

		bool aSep = IsPathSeperator(a[aLength - 2]);
		bool bSep = IsPathSeperator(b[bOffset]);
		if (aSep)
		{
			finalLength--;
		}
		if (bSep)
		{
			finalLength--;
		}
		if (aSep && bSep)
		{
			bOffset++;
		}

		char *result = new char[finalLength];

		int32 resultPlace = 0;

		for (int32 aIter = aOffset; aIter < aLength - 1; aIter++, resultPlace++)
		{
			result[resultPlace] = a[aIter];
		}

		if (!(aSep || bSep))
		{
			result[resultPlace] = '/';
			resultPlace++;
		}

		for (int32 bIter = bOffset; bIter < bLength; bIter++, resultPlace++)
		{
			result[resultPlace] = b[bIter];
		}

		if (outLength)
		{
			*outLength = finalLength;
		}
		return result;
	}

	/**
	\brief Removes the last element from the path, unless you're already up to the drive
	*/
	char *GetParentDirectory(char *path, int32 *outFinalLength = nullptr)
	{
		int32 nameStart = CString::FindLastCharacter(path, "\\/", 2) + 1;
		int32 contentLength = CString::GetLength(path, nameStart) - 1;

		if (contentLength == nameStart)
		{
			contentLength--;
			nameStart = CString::FindLastCharacter(path, "\\/", 2, contentLength) + 1;
		}
		char *result = CString::CopySubstring(path, nameStart, outFinalLength, nameStart, 0);

		return result;
	}

	/**
	\brief Gets the name of the directory or file at the end of the path chain (including extension)
	*/
	char *GetEndName(char *path, int32 *outFinalLength = nullptr)
	{
		int32 nameStart = CString::FindLastCharacter(path, "\\/", 2) + 1;
		int32 contentLength = CString::GetLength(path, nameStart) - 1;

		if (contentLength == nameStart)
		{
			contentLength--;
			nameStart = CString::FindLastCharacter(path, "\\/", 2, contentLength) + 1;
		}
		char *result = CString::CopySubstring(path, contentLength - nameStart, outFinalLength, contentLength, nameStart);

		return result;
	}
}

#endif