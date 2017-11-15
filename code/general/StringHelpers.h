#ifndef HANDMADE_STRING_HELPERS
#define HANDMADE_STRING_HELPERS

#include "../handmade_typedefs.h"

///Gets the starting index of the specified substring, if it is present.  Returns -1 if it is not.
int32 FindSubstring(const char *find, int32 findLength, const char *within, int32 withinLength, int32 startIndex = 0, bool overlappingMatches = false)
{
	if (findLength == 0 || findLength + startIndex > withinLength)
	{
		return -1;
	}

	for (int32 i = startIndex; i < withinLength - findLength; i++)
	{
		if (within[i] == find[0])
		{
			bool found = true;
			for (int32 j = i; j < withinLength - findLength; j++)
			{
				if (within[j] != find[j - i])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				return i;
			}
		}
	}
	return -1;
}

///Gets the number of times the specified substring occurs in the main string.
int32 CountSubstrings(const char *find, int32 findLength, const char *within, int32 withinLength, int32 *last = nullptr, int32 startIndex = 0, bool overlappingMatches = false)
{
	int32 prevLast = 0;
	int32 lastResult = FindSubstring(find, findLength, within, withinLength, startIndex);
	int32 count = 0;
	while (lastResult != -1)
	{
		count++;
		prevLast = lastResult;
		lastResult = FindSubstring(find, findLength, within, withinLength, (overlappingMatches ? lastResult + 1 : lastResult + findLength - 1));
	}
	if (last)
	{
		*last = prevLast;
	}
	return count;
}

///Gets the index of the next end of line.  Returns linesLength if no line break is found.
int32 GetLineEnd(const char *lines, int32 linesLength, int32 startIndex = 0)
{
	for (int32 i = startIndex; i < linesLength; i++)
	{
		if (lines[i] == '\n')
		{
			return i;
		}
		if (i + 1 < linesLength && (lines[i] == '\r' && lines[i+1] == '\n'))
		{
			return i + 1;
		}
	}
	return linesLength;
}

bool IsWhitespace(char character)
{
	//                          Covers \f, \t, \r, \n, and \v
	return character == ' ' || (character >= '\t' && character <= '\r');
}

bool IsDigit(char character)
{
	return character >= '0' && character <= '9';
}

int32 SeekToNonWhitespace(const char *string, int32 length, int32 offset)
{
	for (int32 i = offset; i < length; i++)
	{
		if (!IsWhitespace(string[i]) || string[i] == '\0')
		{
			return i;
		}
	}
	return length;
}

int32 SeekToNewLine(const char *string, int32 length, int32 offset)
{
	for (int32 i = offset; i < length; i++)
	{
		if (string[i] == '\n')
		{
			return i + 1;
		}
	}
	return length;
}

int32 SeekToCharacter(const char *string, char character, int32 length, int32 offset)
{
	for (int32 i = offset; i < length; i++)
	{
		if (string[i] == character)
		{
			return i;
		}
	}
	return length;
}

static bool TryParseInt32(const char *string, int32 length, int32 &outResult, int32 offset = 0, int32 *readFinishIndex = nullptr)
{
	int32 result = 0;
	int32 negate = 1;
	int32 i = SeekToNonWhitespace(string, length, offset);

	if (length <= i)
	{
		return false;
	}

	if (string[i] == '-')
	{
		negate = -1;
		i++;
	}
	else if (string[i] == '+')
	{
		i++;
	}

	if (length <= i || !IsDigit(string[i]))
	{
		return false;
	}

	for (; i < length; i++)
	{
		if (IsDigit(string[i]))
		{
			//TODO(Ian):  What do we do about overflows?
			int8 digit = string[i] - '0';
			result = result * 10 + digit;
		}
		else if (string[i] == ',')
		{
			//TODO(Ian): Decide what to do about the fact that some places reverse periods and commas in numbers
			//           Simply ignoring periods too feels likely to result in unexpected results
			continue;
		}
		else
		{

			break;
		}
	}

	result *= negate;

	if (readFinishIndex)
	{
		*readFinishIndex = i;
	}

	outResult = result;
	return true;
}

#endif