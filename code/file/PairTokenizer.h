#ifndef HANDMADE_PAIRTOKENIZER_H
#define HANDMADE_PAIRTOKENIZER_H

#include "..\handmade_typedefs.h"
#include "..\general\Assert.h"
#include "..\general\StretchyArray.h"
#include "..\general\StringHelpers.cpp"
#include "..\general\memory\StackArena.h"

namespace File
{
	struct ParseError
	{
		int32 ErrorCode;
		int32 Line;
		int32 Depth;
	};

	struct TokenPair
	{
		char *First;
		int32 FirstLength;
		char *Second;
		int32 SecondLength;
	};

	struct PairTokenizer
	{
		/*
		Basic Format example:
		a = b				//First: a  Second: b
		cdef = "abcd = ds"  //First: cdef  Second: abcd = ds
		ref\=\" = 123		//First: ref=" Second: 123
		def dsd = oytoyt    //Invalid, space in first token
		"def dsd" = oytoyt  //Valid

		All non whitespace text must occur within quotes (if any are present at all)
		*/

		StretchyArray<ParseError> errors;
		StretchyArray<TokenPair> parsedData;
		Memory::StackArena *stringStack;

		ParseError ParseToken(char *string, int32 offset, int32 length, TokenPair *output, int32 &outFinalTokenEnd)
		{
			StretchyArray<char> quoteStack = StretchyArray<char>();
			bool parseSpecial = true;
			bool foundNonWhitespace = false;
			bool quoteEnclosed = false;
			uint32 currTokenMemReserve = 0;

			int32 i = CString::FindNonWhitespace(string, length, offset);
			if (CString::IsQuotation(string[i]))
			{
				quoteEnclosed = true;
			}
			for (; i < length; i++)
			{
				bool recordLetter = true;
				if (parseSpecial)
				{
					if (string[i] == '\\') //Skip next special character
					{
						parseSpecial = false;
						recordLetter = false;
					}
					else if (quoteEnclosed && CString::IsQuotation(string[i]))
					{
						if (quoteStack.Length() > 0 && quoteStack[quoteStack.Length() - 1] == string[i])
						{
							quoteStack.PopBack();
							if (quoteStack.Length() == 0) //If we just popped the last quote
							{
								recordLetter = false;
							}
						}
						else
						{
							quoteStack.PushBack(string[i]);
							if (quoteStack.Length() == 1)//If we just found the first quote
							{
								recordLetter = false;
							}
						}
					}
					else if (quoteStack.Length() == 0)
					{
						if (string[i] == '=' || CString::IsWhitespace(string[i]))
						{
							break;
						}
					}
				}
				else
				{
					parseSpecial = true;
				}

				if (recordLetter)
				{
					if (currTokenEnd - currTokenStart + 1 > currTokenMemReserve)
					{
						//Grow memory alloc
					}
				}
			}

			//Finalize token

		}

		ParseError ParsePair(char *string, int32 offset, int32 length, TokenPair *output, int32 &outFinalTokenEnd)
		{

		}

		bool ParseFullString(char *string, int32 tokenStart, int32 tokenEnd, int32 quoteStart = tokenStart, int32 quoteEnd = tokenEnd)
		{

			bool inQuote;
			int32 currLine;
			int32 currColumn;

		}
	};
}

#endif // !HANDMADE_PAIRTOKENIZER_H
