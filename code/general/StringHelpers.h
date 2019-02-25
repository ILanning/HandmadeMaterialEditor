#ifndef HANDMADE_STRINGHELPERS_CPP
#define HANDMADE_STRINGHELPERS_CPP

#include <cstdio>
#include <memory.h>
#include "../handmade_typedefs.h"
#include "Assert.h"

namespace CString
{
	///Gets the starting index of the specified substring, if it is present.  Returns -1 if it is not.
	inline int32 FindSubstring(const char *find, const int32 findLength, const char *within, const int32 withinLength)
	{
		if (findLength == 0 || findLength > withinLength)
		{
			return -1;
		}

		for (int32 i = 0; i < withinLength - findLength + 1; i++)
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

	inline int32 FindSubstring(const char *find, const int32 findLength, const char *within, const int32 withinLength, int32 offset)
	{
		int32 result = FindSubstring(find, findLength, within + offset, withinLength - offset);
		if(result == -1)
		{
			return -1;
		}
		else
		{
			return result + offset;
		}
	}

	///Gets the number of times the specified substring occurs in the main string.
	inline int32 CountSubstrings(const char *find, const int32 findLength, const char *within, const int32 withinLength, int32 *last = nullptr, bool overlappingMatches = false)
	{
		int32 prevLast = 0;
		int32 lastResult = FindSubstring(find, findLength, within, withinLength);
		int32 count = 0;
		while (lastResult != -1)
		{
			count++;
			prevLast = lastResult;

			int32 nextSearchStart = 0;
			if (overlappingMatches)
			{
				nextSearchStart = lastResult + 1;
			}
			else
			{
				nextSearchStart = lastResult + findLength - 1;
			}

			lastResult = FindSubstring(find + nextSearchStart, findLength, within, withinLength);
		}
		if (last)
		{
			*last = prevLast;
		}
		return count;
	}

	inline char *CopySubstring(const char *source, const int32 copyLength, int32 *outFinalLength = nullptr, const int32 sourceLength = MaxInt32)
	{
		if (!source)
		{
			if (outFinalLength)
			{
				*outFinalLength = 0;
			}
			return nullptr;
		}
		int32 finalLength = copyLength;
		if (sourceLength != MaxInt32)
		{
			const int32 overflow = sourceLength - (copyLength);
			finalLength = (overflow >= 0) ? copyLength : copyLength + overflow;
		}
		finalLength++;

		//CONSIDER:  Maybe move back the start point when given negative lengths, like
		//			 some other languages do with these sorts of functions
		Assert(finalLength >= 0);

		char *result = new char[finalLength];

		for (int32 i = 0; i < finalLength - 1; i++)
		{
			result[i] = source[i];
		}
		result[finalLength - 1] = '\0';

		if (outFinalLength)
		{
			*outFinalLength = finalLength;
		}

		return result;
	}

	///Writes the given strings to the destination string sequentially.  Does not add a null terminator.
	inline int32 CombineStringsToDest(const char* stringA, const int32 aLength, const char* stringB, const int32 bLength, char* resultString, const int32 resultLength)
	{
		const int32 finalLength = aLength + bLength < resultLength ? aLength + bLength : resultLength;
		const int32 firstCopyLength = aLength < finalLength ? aLength : finalLength;

		int32 i = 0;
		for (; i < firstCopyLength; i++)
		{
			resultString[i] = stringA[i];
		}
		for (; i < finalLength; i++)
		{
			resultString[i] = stringB[i - firstCopyLength];
		}

		return finalLength;
	}

	inline bool IsWhitespace(const char character)
	{
		//                          Covers \f, \t, \r, \n, and \v
		return character == ' ' || (character >= '\t' && character <= '\r');
	}

	inline bool IsQuotation(const char character)
	{
		return character == '\"' || character == '\'';
	}

	inline bool IsBrace(const char character)
	{
		return character == '{' || character == '}' || character == '[' || character == ']' || character == '(' || character == ')';
	}

	inline bool IsDigit(const char character)
	{
		return character >= '0' && character <= '9';
	}

	inline bool IsLetter(const char character)
	{
		return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
	}

	inline bool IsAlphanumeric(const char character)
	{
		return IsDigit(character) || IsLetter(character);
	}

	/** Gets the length of the passed in string, including the null terminator.
	*/
	inline int32 GetLength(const char *string)
	{
		if(!string)
		{ 
			return 0;
		}

		int32 i = 0;
		while (string[i] != '\0')
		{
			i++;
		}
		i++;

		return i;
	}

	inline int32 FindCharacter(const char *string, const char *characters, const int32 characterCount, const int32 length = MaxInt32, const int32 offset = 0)
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

	inline int32 FindCharacter(const char *string, const char character, const int32 length = MaxInt32, const int32 offset = 0)
	{
		return FindCharacter(string, &character, 1, length, offset);
	}

	inline int32 FindLastCharacter(const char *string, const char *characters, const int32 characterCount, const int32 length = MaxInt32, const int32 offset = 0)
	{
		Assert(string != nullptr);

		int32 result = -1;

		if (length == MaxInt32)
		{
			int32 i = offset;
			while (string[i] != '\0')
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
		}
		else
		{
			for (int32 i = length - 1; i >= offset; i--)
			{
				for (int32 j = 0; j < characterCount; j++)
				{
					if (string[i] == characters[j])
					{
						result = i;
						i = offset - 1; //Bail out of the parent loop too
						break;
					}
				}
			}
		}

		return result;
	}

	inline int32 FindLastCharacter(const char *string, const char character, const int32 length = MaxInt32, const int32 offset = 0)
	{
		return FindLastCharacter(string, &character, 1, length, offset);
	}

	inline int32 FindNonWhitespace(const char *string, const int32 length = MaxInt32, const int32 offset = 0)
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
	inline int32 FindLineEnd(const char *string, const int32 length = MaxInt32, const int32 offset = 0)
	{
		int32 result = FindCharacter(string, '\n', length, offset);
		if (result > 0 && string[result - 1] == '\r')
		{
			result--;
		}
		return result;
	}

	///Gets the index of the first character after the next end of line.  Returns -1 if no line break is found.
	inline int32 FindNextLineStart(const char *string, const int32 length = MaxInt32, const int32 offset = 0)
	{
		int32 result = FindCharacter(string, '\n', length, offset);
		if (result != -1)
		{
			result++;
		}
		return result;
	}

	inline char LowerChar(const char character)
	{
		if (character >= 'A' && character <= 'Z')
		{
			return character + ('a' - 'A');
		}

		return character;
	}

	inline void EditToLower(char *string, const int32 length = MaxInt32)
	{
		for (int32 i = 0; length == MaxInt32 ? string[i] != '/0' : i < length; i++)
		{
			string[i] = LowerChar(string[i]);
		}
	}

	inline char UpperChar(const char character)
	{
		if (character >= 'a' && character <= 'z')
		{
			return character - ('a' - 'A');
		}

		return character;
	}

	inline void EditToUpper(char *string, const int32 length = MaxInt32)
	{
		for (int32 i = 0; length == MaxInt32 ? string[i] != '/0' : i < length; i++)
		{
			string[i] = UpperChar(string[i]);
		}
	}

	inline bool TryParseInt32(const char *string, const int32 stringLength, int32 &outResult, int32 offset = 0, int32 *readFinishIndex = nullptr)
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
				//TODO:  What do we do about overflows?
				int8 digit = string[i] - '0';
				result = result * 10 + digit;
			}
			else if (string[i] == ',')
			{
				//TODO: Decide what to do about the fact that some places reverse periods and commas in numbers
				//      Simply ignoring periods too feels likely to result in unexpected results
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

	inline bool TryParseBool(const char *string, const int32 stringLength, bool &outResult, int32 offset = 0, int32 *readFinishIndex = nullptr)
	{
		/*
		Valid true:    	 Valid false:
		true			 false
		t				 f
		yes				 no
		y                n
		on				 off
		*/

		bool result = false;
		bool success = true;
		int32 stringPos = FindNonWhitespace(string, stringLength, offset);

		const int32 shortestValidLength = 5;
		const int32 bufferSize = shortestValidLength < stringLength - stringPos ? shortestValidLength : stringLength - stringPos;
		char buffer[shortestValidLength];

		if (bufferSize <= 0)
		{
			return false;
		}

		for (int32 bufferPos = 0; bufferPos < bufferSize; bufferPos++)
		{
			buffer[bufferPos] = LowerChar(string[stringPos + bufferPos]);
		}

		int32 bufferPos = 0;
		if (buffer[bufferPos] == 't')
		{
			if (bufferSize >= 4 && FindSubstring(buffer + 1, 3, "rue", 3))
			{
				stringPos += 4;
			}
			else
			{
				stringPos++;
			}
			result = true;
		}
		else if (buffer[bufferPos] == 'y')
		{
			if (bufferSize >= 3 && FindSubstring(buffer + 1, 2, "es", 2))
			{
				stringPos += 3;
			}
			else
			{
				stringPos++;
			}
			result = true;
		}
		else if (buffer[bufferPos] == 'f')
		{
			if (bufferSize >= 5 && FindSubstring(buffer + 1, 4, "alse", 4))
			{
				stringPos += 5;
			}
			else
			{
				stringPos++;
			}
		}
		else if (buffer[bufferPos] == 'n')
		{
			if (bufferSize >= 2 && buffer[bufferPos + 1] == 'o')
			{
				stringPos += 2;
			}
			else
			{
				stringPos++;
			}
		}
		else if (buffer[bufferPos] == 'o')
		{
			if (bufferSize >= 2 && buffer[bufferPos + 1] == 'n')
			{
				stringPos += 2;
				result = true;
			}
			else if (bufferSize >= 3 && FindSubstring(buffer + 1, 2, "ff", 2))
			{
				stringPos += 3;
			}
			else
			{
				success = false;
			}
		}
		else
		{
			success = false;
		}

		if (readFinishIndex)
		{
			*readFinishIndex = stringPos;
		}

		outResult = result;
		return success;
	}

	/** Writes an int value to the given string, using as little space as possible.  Returns the number of characters used.
	*/
	inline int32 WriteInt(char* string, const int32 stringLength, const int32 value, const int32 maxCharsUsed)
	{
		char buffer[12];

		snprintf(buffer, 12, "%d", value);

		int32 charsNeeded = 0;

		while (buffer[charsNeeded] != '\0')
		{
			if (charsNeeded < maxCharsUsed && charsNeeded < stringLength)
			{
				string[charsNeeded] = buffer[charsNeeded];
			}

			charsNeeded++;
		}

		return charsNeeded;
	}

	///Writes one string into another, clipping the string to the shortest length.
	inline int32 WriteChars(char* target, const int32 targetLength, const char* source, const int32 sourceLength)
	{
		int32 writeAmount = targetLength <= sourceLength ? targetLength : sourceLength;
		memcpy(target, source, writeAmount);
		return writeAmount;
	}

	/** Writes either "true" or "false" to the given string depending on the input.  Returns the number of characters used.
	*/
	inline int32 WriteBool(char* string, const int32 stringLength, const bool value)
	{
		if (value && 4 < stringLength)
		{
			WriteChars(string, stringLength, "true", 4);
			return 4;
		}
		else if (!value && 5 < stringLength)
		{
			WriteChars(string, stringLength, "false", 5);
			return 5;
		}

		return 0;
	}

	inline bool IsEqual(const char* a, const char* b)
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

	inline bool IsEqual(const char* a, const char* b, const int32 aLength, const int32 bLength)
	{
		if (aLength != bLength)
		{
			return false;
		}
		if (a == b)
		{
			return true;
		}
		if (a == nullptr || b == nullptr)
		{
			return false;
		}
		for (int32 i = 0; i < aLength; i++)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}

		return true;
	}
}

#endif //HANDMADE_STRINGHELPERS_CPP