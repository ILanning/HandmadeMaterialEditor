#ifndef HANDMADE_STRING_HELPERS
#define HANDMADE_STRING_HELPERS

#include "../handmade_typedefs.h"
#include "../handmade.h"

namespace CString
{
	///Gets the starting index of the specified substring, if it is present.  Returns -1 if it is not.
	int32 FindSubstring(const char *find, int32 findLength, const char *within, int32 withinLength, int32 startIndex = 0)
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
				for (int32 j = 0; j < findLength; j++)
				{
					if (within[i + j] != find[j])
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


	char *CopySubstring(const char *source, int32 copyLength, int32 *outFinalLength = nullptr, int32 sourceLength = MaxInt32, int32 offset = 0)
	{
		int32 overflow = sourceLength - (copyLength + offset);
		int32 finalLength = (overflow >= 0) ? copyLength : copyLength + overflow;
		finalLength++;

		//CONSIDER(Ian):  Maybe move back the start point when given negative lengths, like
		//				  some other languages do with these sorts of functions
		Assert(finalLength >= 0);

		char *result = new char[finalLength];

		for (int32 i = 0; i < finalLength - 1; i++)
		{
			result[i] = source[i + offset];
		}
		result[finalLength - 1] = '\0';

		if (outFinalLength)
		{
			*outFinalLength = finalLength;
		}

		return result;
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

	bool IsLetter(char character)
	{
		return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
	}

	bool IsAlphanumeric(char character)
	{
		return IsDigit(character) || IsLetter(character);
	}

	/*
		\bried Gets the lengths of the passed in string, including the null terminator.
	*/
	int32 GetLength(const char *string, int32 offset = 0)
	{
		Assert(string != nullptr);

		int32 i = offset;
		while (string[i] != '\0')
		{
			i++;
		}
		i++;

		return i;
	}

	int32 FindCharacter(const char *string, const char *characters, int32 characterCount, int32 length = MaxInt32, int32 offset = 0)
	{
		Assert(string != nullptr);

		int32 i = offset;
		while (length != MaxInt32 ? i < length : string[i] != '\0')
		{
			for (int32 j = 0; j < characterCount; j++)
			{
				if (string[i] == characters[j])
				{
					return i;
				}
			}
			i++;
		}
		return -1;
	}

	int32 FindCharacter(const char *string, char character, int32 length = MaxInt32, int32 offset = 0)
	{
		//TODO(Ian): Test performance here, would it be better to have two versions?
		return FindCharacter(string, &character, 1, length, offset);
	}

	int32 FindLastCharacter(const char *string, const char *characters, int32 characterCount, int32 length = MaxInt32, int32 offset = 0)
	{
		//TODO(Ian): Make this start from the back of strings of known length
		Assert(string != nullptr);

		int32 i = offset;
		int32 result = -1;
		while (length != MaxInt32 ? i < length : string[i] != '\0')
		{
			for (int32 j = 0; j < characterCount; j++)
			{
				if (string[i] == characters[j])
				{
					result = i;
				}
			}
			i++;
		}
		return result;
	}

	int32 FindLastCharacter(const char *string, char character, int32 length = MaxInt32, int32 offset = 0)
	{
		return FindLastCharacter(string, &character, 1, length, offset);
	}

	int32 FindNonWhitespace(const char *string, int32 length = MaxInt32, int32 offset = 0)
	{
		for (int32 i = offset; i < length; i++)
		{
			if (length == MaxInt32  && string[i] == '\0')
			{
				break;
			}
			if (!IsWhitespace(string[i]))
			{
				return i;
			}
		}
		return -1;
	}

	///Gets the index of the next end of line.  Returns -1 if no line break is found.
	int32 FindLineEnd(const char *string, int32 length = MaxInt32, int32 offset = 0)
	{
		return FindCharacter(string, '\n', length, offset);
	}

	void EditToLower(char *string, int32 count = MaxInt32, int32 length = MaxInt32, int32 offset = 0)
	{
		int32 finalValue = count + offset;
		if (finalValue > length)
		{
			finalValue -= finalValue - length;
		}
		if (finalValue <= 0)
		{
			return;
		}

		for (int32 i = offset; i < finalValue; i++)
		{
			if (string[i] >= 'A' && string[i] <= 'Z')
			{
				string[i] += 'a' - 'A';
			}
		}
	}

	void EditToUpper(char *string, int32 stringLength, int32 editCount, int32 offset = 0)
	{
		int32 finalValue = editCount + offset;
		if (finalValue > stringLength)
		{
			finalValue -= finalValue - stringLength;
		}
		if (finalValue <= 0)
		{
			return;
		}

		for (int32 i = offset; i < finalValue; i++)
		{
			if (string[i] >= 'a' && string[i] <= 'z')
			{
				string[i] -= 'a' - 'A';
			}
		}
	}

	bool TryParseInt32(const char *string, int32 stringLength, int32 &outResult, int32 offset = 0, int32 *readFinishIndex = nullptr)
	{
		int32 result = 0;
		int32 negate = 1;
		int32 i = FindNonWhitespace(string, stringLength, offset);

		if (stringLength <= i)
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

		if (stringLength <= i || !IsDigit(string[i]))
		{
			return false;
		}

		for (; i < stringLength; i++)
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

	bool IsEqual(const char *a, const char *b)
	{
		if (a == b)
		{
			return true;
		}
		if (a == nullptr || b == nullptr)
		{
			return false;
		}

		int32 i = 0;
		do
		{
			if (a[i] != b[i])
			{
				return false;
			}
			i++;
		} while (a[i - 1] != '\0');

		return true;
	}

	bool IsEqual(const char *a, const char *b, int32 aLength, int32 bLength, int32 aOffset = 0, int32 bOffset = 0)
	{
		if (aLength - aOffset != bLength - bOffset)
		{
			return false;
		}
		if (a == b && aOffset == bOffset)
		{
			return true;
		}
		if (a == nullptr || b == nullptr)
		{
			return false;
		}

		int32 aIter = aOffset;
		int32 bIter = bOffset;
		for (; aIter < aLength; aIter++, bIter++)
		{
			if (a[aIter] != b[bIter])
			{
				return false;
			}
		}

		return true;
	}
}

#endif