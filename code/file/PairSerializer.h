#ifndef HANDMADE_PAIRSERIALIZER_H
#define HANDMADE_PAIRSERIALIZER_H

#include "..\handmade_typedefs.h"
#include "..\handmade_funcdefs.h"
#include "..\general\ArrayList.h"
#include "..\general\Assert.h"
#include "..\general\HashMap.h"
#include "..\general\HMPair.h"
#include "..\general\HMString.h"
#include "..\general\StaticArray.h"
#include "..\general\StringHelpers.cpp"

namespace File
{
	///Functions for writing and reading simple dictionary-like data structures from file
	namespace PairSerializer
	{
		///Potential parsing errors the serializer may find
		enum class ErrorCodes
		{
			///No error
			OK = 0,
			///A qoutation mark was found, but no match occurred between it and the end of the document
			QuoteDidNotEnd,
			///Two tokens in a pair were found to be seperated by a space rather than an equals sign
			NoEqualsBeforeNextToken,
		};

		///Contains information on a found parsing error.
		struct ParseError
		{
			///The error code for this issue
			ErrorCodes ErrorCode = ErrorCodes::OK;
			///The line the error is believed to be on
			int32 Line = 0;
			///The column/character depth into the line of the error.
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

		typedef HMPair<HMString, HMString> TokenPair;

		///Parses the next single token from the given text, allocating memory for the token string.
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

		///Returns the next single pair found in the given text, allocating memory for the resulting strings.  
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

		///Returns a HashMap containing all pairs found in the given text and creates copies of the strings for it.
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

		///Returns a HashMap containing all pairs found in the given file and creates copies of the strings for it.
		template<class Allocator>
		Collections::HashMap<HMString, HMString, Allocator> ParseFile(HMString path, ReadFileFunc& readFile, Allocator& memory, bool* outSuccess = nullptr)
		{
			bool success = true;
			FileData fileData = readFile(path.RawCString(), path.Length(), &success);

			if (outSuccess)
			{
				*outSuccess = success;
			}

			if (success)
			{
				return ParseText({ (char*)fileData.File, fileData.FileSize }, memory);
			}
			return {};
		}

		///Takes a HashMap and creates a file containing a human-readable array of pairs from it.
		template<class Allocator>
		bool WriteFile(HMString path, WriteFileFunc& writeFile, StaticArray<HMPair<HMString, HMString>> data, Allocator& scratchAlloc)
		{
			int32 totalLength = 1;
			for (uint32 i = 0; i < data.Length; i++)
			{
				totalLength += data[i].First.Length() + data[i].Second.Length() + 5;
			}

			char* fileText = scratchAlloc.Allocate<void>(totalLength);

			itn32 fileTextIter = 0;
			for (uint32 arrayIter = 0; arrayIter < data.Length; arrayIter++)
			{
				firstLength = data[arrayIter].First.Length() - 1;
				secondLength = data[arrayIter].Second.Length() - 1;

				for (uint32 firstIter = 0; firstIter < firstLength; firstIter++)
				{
					fileText[fileTextIter++] = data[arrayIter].First[firstIter];
				}

				fileText[fileTextIter++] = ' ';
				fileText[fileTextIter++] = '=';
				fileText[fileTextIter++] = ' ';

				for (uint32 secondIter = 0; secondIter < secondLength; secondIter++)
				{
					fileText[fileTextIter++] = data[arrayIter].First[secondIter];
				}

				fileText[fileTextIter++] = '\r';
				fileText[fileTextIter++] = '\n';
			}

			bool success = true;
			writeFile(path.RawCString(), path.Length(), fileText, totalLength, &success);

			return success;
		}
	}
}

#endif // HANDMADE_PAIRSERIALIZER_H
