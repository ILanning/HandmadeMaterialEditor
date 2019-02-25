#ifndef HANDMADE_PATHHELPERS_CPP
#define HANDMADE_PATHHELPERS_CPP

#include "../handmade_typedefs.h"
#include "StringHelpers.h"
#include "PathHelpers.h"

namespace Path
{
	bool IsAbsolute(const char *string)
	{
		if (!string) //CONSIDER: Do we want this to just crash if it's given a null pointer?
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

	bool IsRelative(const char *string)
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

	char *Combine(const char *a, const  char* b, int32 aOffset, int32 bOffset, int32 *outLength)
	{
		Assert(IsRelative(b));
		Assert(a != nullptr);
		Assert(b != nullptr);

		int32 aLength = CString::GetLength(a);
		int32 bLength = CString::GetLength(b);

		//Null terminator from 'a' becomes a path seperator
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

	char *CloneParentDirectory(const char *path, int32 *outFinalLength)
	{
		int32 lastSeperators[2] = { -1, -1 };
		int32 i = 0;
		bool stillInSeperator = false;
		while (path[i] != '\0')
		{
			if (IsPathSeperator(path[i]))
			{
				if (!stillInSeperator)
				{
					lastSeperators[0] = lastSeperators[1];
					lastSeperators[1] = i;
				}
				stillInSeperator = true;
			}
			else
			{
				stillInSeperator = false;
			}
			i++;
		}

		int32 parentLength;
		if (lastSeperators[1] == -1) //ex: "directory"
		{
			parentLength = 0;
		}
		else if (stillInSeperator)
		{
			if (lastSeperators[0] == -1) //ex: "directory/", "directory//", etc.
			{
				parentLength = 0;
			}
			else
			{
				parentLength = lastSeperators[0] + 1;
			}
		}
		else
		{
			parentLength = lastSeperators[1] + 1;
		}

		char* result = CString::CopySubstring(path, parentLength, outFinalLength);
		return result;
	}

	char *CloneEndName(const char *path, int32 *outFinalLength)
	{
		/*int32 nameStart = CString::FindLastCharacter(path, "\\/", 2) + 1;
		int32 contentLength = CString::GetLength(path + nameStart) - 1;

		if (contentLength == nameStart)
		{
			contentLength--;
			nameStart = CString::FindLastCharacter(path, "\\/", 2, contentLength) + 1;
		}
		char *result = CString::CopySubstring(path + nameStart, contentLength, outFinalLength);

		return result;*/

		int32 lastSeperators[2] = { -1, -1 };
		int32 i = 0;
		bool stillInSeperator = false;
		while (path[i] != '\0')
		{
			if (IsPathSeperator(path[i]))
			{
				if (!stillInSeperator)
				{
					lastSeperators[0] = lastSeperators[1];
					lastSeperators[1] = i;
				}
				stillInSeperator = true;
			}
			else
			{
				stillInSeperator = false;
			}
			i++;
		}

		int32 startingPoint = 0;
		int32 nameLength;
		if (lastSeperators[1] == -1) //ex: "directory"
		{
			nameLength = i;
		}
		else if (stillInSeperator) //ex: "directory///"
		{
			nameLength = lastSeperators[1];
			if (lastSeperators[0] != -1) //ex: "/directory/", "parent///directory//", etc.
			{
				while (IsPathSeperator(path[lastSeperators[0]]))
				{
					lastSeperators[0]++;
				}
				startingPoint = lastSeperators[0];
				nameLength -= startingPoint;
			}
		}
		else
		{
			while (IsPathSeperator(path[lastSeperators[1]]))
			{
				lastSeperators[1]++;
			}
			nameLength = i - lastSeperators[1];
			startingPoint = lastSeperators[1];
		}

		char* result = CString::CopySubstring(path + startingPoint, nameLength, outFinalLength);
		return result;
	}
}

#endif //HANDMADE_PATHHELPERS_CPP