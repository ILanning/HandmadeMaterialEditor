#ifndef HANDMADE_PAIRTOKENIZER_H
#define HANDMADE_PAIRTOKENIZER_H

#include "..\handmade_typedefs.h"
#include "..\general\Assert.h"
#include "..\general\HashMap.h"
#include "..\general\HMString.h"
#include "..\general\ArrayList.h"
#include "..\general\StaticArray.h"
#include "..\general\StringHelpers.cpp"

namespace File
{
	namespace PairTokenizer
	{
		enum class ErrorCodes
		{
			OK = 0,
			QuoteDidNotEnd,
			NoEqualsBeforeNextToken,
		};

		struct ParseError
		{
			ErrorCodes ErrorCode = ErrorCodes::OK;
			int32 Line = 0;
			int32 Depth = 0;

			bool operator ==(const ParseError& other) const
			{
				if (this == &other ||
					(ErrorCode == other.ErrorCode &&
					 Line == other.Line &&
					 Depth == other.Depth))
				{
					return true;
				}

				return false;
			}

			bool operator !=(const ParseError& other) const
			{
				return !(*this == other);
			}
		};

		struct TokenPair
		{
			HMString First;
			HMString Second;
		};

		// Parses the next single token from the given text, allocating memory for the token string.
		template<class Allocator>
		HMString ParseToken(const HMString& string, Allocator& allocator, int32 offset = 0, int32* outOffset = nullptr, ParseError* outError = nullptr)
		{
			auto tokenBuilder = Collections::ArrayList<char, Allocator>(&allocator, 16);
			bool parseSpecial = true;
			char quoteChar = '\0';

			int32 i = CString::FindNonWhitespace(string.RawCString(), string.Length(), offset);

			while (string[i] == '#')
			{
				i = CString::FindLineEnd(string.RawCString(), string.Length(), i);
				i = CString::FindNonWhitespace(string.RawCString(), string.Length(), i);
			}

			if (CString::IsQuotation(string[i]))
			{
				quoteChar = string[i];
				i++;
			}

			for (; i < (int32)string.Length(); i++)
			{
				bool recordLetter = true;
				if (parseSpecial)
				{
					if (string[i] == '\\') //Treat next character as a normal non-whitespace character
					{
						parseSpecial = false;
						recordLetter = false;
					}
					else if (quoteChar != '\0')
					{
						if (string[i] == quoteChar)
						{
							quoteChar = '\0';
							i++;
							break;
						}
					}
					else if (string[i] == '=' || CString::IsWhitespace(string[i]))
					{
						break;
					}
				}
				else
				{
					parseSpecial = true;
				}

				if (recordLetter)
				{
					tokenBuilder.Add(string[i]);
				}
			}

			if (outError)
			{
				if (quoteChar != '\0')
				{
					int32 firstQuote = CString::FindNonWhitespace(string.RawCString(), string.Length(), offset);
					*outError = {ErrorCodes::QuoteDidNotEnd, 0, firstQuote };
				}
				else
				{
					*outError = {};
				}
			}

			if (outOffset)
			{
				*outOffset = i;
			}
			
			if (tokenBuilder[tokenBuilder.Length() - 1] != '\0')
			{
				tokenBuilder.Add('\0');
			}

			HMString result = HMString(tokenBuilder.ToArray(allocator), tokenBuilder.Length());
			return result;
		}

		// Returns the next single pair found in the given text, allocating memory for the resulting strings.  
		template<class Allocator>
		TokenPair ParsePair(const HMString& string, Allocator& allocator, int32 offset = 0, int32* outOffset = nullptr, ParseError* outError = nullptr)
		{
			TokenPair result = {};
			ParseError error = {};

			result.First = ParseToken(string, allocator, offset, &offset, &error);

			if (error.ErrorCode == ErrorCodes::OK)
			{
				offset = CString::FindNonWhitespace(string.RawCString(), string.Length(), offset);

				if (string[offset] != '=')
				{
					error = { ErrorCodes::NoEqualsBeforeNextToken, 0, offset };
				}
				else
				{
					offset++;
					result.Second = ParseToken(string, allocator, offset, &offset, &error);
				}
			}

			if (outOffset)
			{
				*outOffset = offset;
			}
			if (outError)
			{
				*outError = error;
			}

			return result;
		}

		// Returns a HashMap containing all pairs found in the given text and creates copies of the strings for it.
		template<class Allocator>
		Collections::HashMap<HMString, HMString, Allocator> ParseText(const HMString& string, Allocator& allocator, int32 offset = 0, StaticArray<ParseError>* outErrors = nullptr)
		{
			int32 lineNum = 0;
			int32 lastLineBound = 0;
			auto resultDict = Collections::HashMap<HMString, HMString, Allocator>(&allocator);
			auto errorList = Collections::ArrayList<ParseError, Allocator>(&allocator);

			while (offset < (int32)string.Length())
			{
				ParseError error = {};
				int32 upcomingOffset = 0;
				TokenPair pair = ParsePair(string, allocator, offset, &upcomingOffset, &error);

				if (error.ErrorCode != ErrorCodes::OK)
				{
					int32 upcomingLine = lastLineBound;
					while (upcomingLine < upcomingOffset && upcomingLine != -1)
					{
						lastLineBound = upcomingLine;
						lineNum++;
						upcomingLine = CString::FindNextLineStart(string.RawCString(), string.Length(), lastLineBound);
					}
					error.Line = lineNum;
					error.Depth -= lastLineBound;

					errorList.Add(error);
				}

				offset = upcomingOffset;

				resultDict[pair.First] = pair.Second;

			}

			if (outErrors)
			{
				*outErrors = errorList.ToStaticArray(allocator);
			}

			return resultDict;
		}
	}
}

#endif // !HANDMADE_PAIRTOKENIZER_H
