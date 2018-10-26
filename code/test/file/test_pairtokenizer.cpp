#ifndef HANDMADE_TEST_PAIRTOKENIZER
#define HANDMADE_TEST_PAIRTOKENIZER

#include "../../general/memory/NewDeleteArena.h"
#include "../../general/HMString.h"
#include "../../general/StaticArray.h"
#include "../../file/PairTokenizer.h"

namespace TestPairTokenizerHelpers
{
	void CheckAndFreeToken(HMString token, HMString expected, Memory::NewDeleteArena arena)
	{
		CHECK(token == expected);
		arena.Deallocate(token.RawCString());
	}

	void CheckAndFreePair(File::PairTokenizer::TokenPair pair, File::PairTokenizer::TokenPair expected, Memory::NewDeleteArena arena)
	{
		CheckAndFreeToken(pair.First, expected.First, arena);
		CheckAndFreeToken(pair.Second, expected.Second, arena);
	}

	void CheckDictSet(HMString key, HMString value, Collections::HashMap<HMString, HMString, Memory::NewDeleteArena>& dict)
	{
		CHECK(dict.CheckExists(key));
		CHECK(dict[key] == value);
	}
}

TEST_CASE("Testing PairTokenizer")
{
	Memory::NewDeleteArena arena = Memory::NewDeleteArena();
	/*
	Format examples:
	a = b a=d			//First: a  Second: b    First: a   Second: d
	cdef = "abcd = ds"  //First: cdef  Second: abcd = ds
	ref\=\\\" = 123		//First: ref=\" Second: 123
	def dsd = oytoyt    //Invalid, space in first token
	"def dsd" = oytoyt  //Valid
	#This is a comment  //Ignored
	#also comment       //Ignored
	'fdgfr' = #comment
		\#gdddddg       //First: fdgfr  Second: #gdddddg
	*/

	char* testData = "a = b a=d\r\ncdef = \"abcd = ds\"\r\nref\\=\\\\\\\" = 123\r\ndef dsd = oytoyt\r\n\"def dsd\" = oytoyt\r\n#This is a comment\r\n#also comment\r\n'fdgfr' = #comment\r\n   \\#gdddddg";

	HMString testString = HMString(testData, 154);
	
	SUBCASE("FindToken")
	{
		HMString result = File::PairTokenizer::ParseToken(testString, arena);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { "a" }, arena);

		result = File::PairTokenizer::ParseToken(testString, arena, 3);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { "b" }, arena);

		result = File::PairTokenizer::ParseToken(testString, arena, 9);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { "cdef" }, arena);

		result = File::PairTokenizer::ParseToken(testString, arena, 17);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { "abcd = ds" }, arena);

		result = File::PairTokenizer::ParseToken(testString, arena, 29);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { "ref=\\\"" }, arena);

		File::PairTokenizer::ParseError error = {};
		HMString errorTestString = { "\" = oytoyt" };
		result = File::PairTokenizer::ParseToken(errorTestString, arena, 0, nullptr, &error);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { " = oytoyt" }, arena);
		CHECK(error.ErrorCode == File::PairTokenizer::ErrorCodes::QuoteDidNotEnd);

		result = File::PairTokenizer::ParseToken(testString, arena, 86);
		TestPairTokenizerHelpers::CheckAndFreeToken(result, { "fdgfr" }, arena);
	}

	SUBCASE("ParsePair")
	{
		int32 offset = 0;
		auto result = File::PairTokenizer::ParsePair(testString, arena, 0, &offset);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {"a"}, {"b"} }, arena);

		result = File::PairTokenizer::ParsePair(testString, arena, offset, &offset);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {"a"}, {"d"} }, arena);

		result = File::PairTokenizer::ParsePair(testString, arena, offset, &offset);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {"cdef"}, {"abcd = ds"} }, arena);

		result = File::PairTokenizer::ParsePair(testString, arena, offset, &offset);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {"ref=\\\""}, {"123"} }, arena);

		File::PairTokenizer::ParseError error = {};
		result = File::PairTokenizer::ParsePair(testString, arena, offset, &offset, &error);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {"def"}, {} }, arena);
		CHECK(error.ErrorCode == File::PairTokenizer::ErrorCodes::NoEqualsBeforeNextToken);

		HMString errorTestString = { "\" = oytoyt" };
		result = File::PairTokenizer::ParsePair(errorTestString, arena, 0, nullptr, &error);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {" = oytoyt"}, {} }, arena);
		CHECK(error.ErrorCode == File::PairTokenizer::ErrorCodes::QuoteDidNotEnd);

		result = File::PairTokenizer::ParsePair(testString, arena, 86, nullptr);
		TestPairTokenizerHelpers::CheckAndFreePair(result, { {"fdgfr"}, {"#gdddddg"} }, arena);
	}

	SUBCASE("ParseText")
	{
		int32 offset = 0;
		auto result = File::PairTokenizer::ParseText({ "a = b" }, arena);
		TestPairTokenizerHelpers::CheckDictSet({ "a" }, { "b" }, result);

		result = File::PairTokenizer::ParseText({testData, 46}, arena);
		TestPairTokenizerHelpers::CheckDictSet({ "a" }, { "d" }, result);
		TestPairTokenizerHelpers::CheckDictSet({ "cdef" }, { "abcd = ds" }, result);
		TestPairTokenizerHelpers::CheckDictSet({ "ref=\\\"" }, { "123" }, result);

		result = File::PairTokenizer::ParseText({ testData + 65 }, arena); 
		TestPairTokenizerHelpers::CheckDictSet({ "def dsd" }, { "oytoyt" }, result);
		TestPairTokenizerHelpers::CheckDictSet({ "fdgfr" }, { "#gdddddg" }, result);

		StaticArray<File::PairTokenizer::ParseError> errors = {};
		result = File::PairTokenizer::ParseText(testString, arena, 0, &errors);
		File::PairTokenizer::ParseError expectedError = { File::PairTokenizer::ErrorCodes::NoEqualsBeforeNextToken, 4, 4 };
		CHECK(errors[0] == expectedError);
		TestPairTokenizerHelpers::CheckDictSet({ "a" }, { "d" }, result);
		TestPairTokenizerHelpers::CheckDictSet({ "def" }, {}, result);
		TestPairTokenizerHelpers::CheckDictSet({ "dsd" }, { "oytoyt" }, result);
		TestPairTokenizerHelpers::CheckDictSet({ "fdgfr" }, { "#gdddddg" }, result);
	}
}

#endif //HANDMADE_TEST_PAIRTOKENIZER