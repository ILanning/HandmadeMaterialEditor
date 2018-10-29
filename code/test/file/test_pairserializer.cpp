#ifndef HANDMADE_TEST_PAIRTOKENIZER
#define HANDMADE_TEST_PAIRTOKENIZER

#include "../../general/memory/NewDeleteArena.h"
#include "../../general/HMString.h"
#include "../../general/StaticArray.h"
#include "../../file/PairSerializer.h"

namespace TestPairSerializerHelpers
{
	void CheckAndFreeToken(HMString token, HMString expected, Memory::NewDeleteArena arena)
	{
		CHECK(token == expected);
		arena.Deallocate(token.RawCString());
	}

	void CheckAndFreePair(File::PairSerializer::TokenPair pair, File::PairSerializer::TokenPair expected, Memory::NewDeleteArena arena)
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
	using namespace File::PairSerializer;
	using namespace TestPairSerializerHelpers;

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
		HMString result = ParseToken(testString, arena);
		CheckAndFreeToken(result, { "a" }, arena);

		result = ParseToken(testString, arena, 3);
		CheckAndFreeToken(result, { "b" }, arena);

		result = ParseToken(testString, arena, 9);
		CheckAndFreeToken(result, { "cdef" }, arena);

		result = ParseToken(testString, arena, 17);
		CheckAndFreeToken(result, { "abcd = ds" }, arena);

		result = ParseToken(testString, arena, 29);
		CheckAndFreeToken(result, { "ref=\\\"" }, arena);

		ParseError error = {};
		HMString errorTestString = { "\" = oytoyt" };
		result = ParseToken(errorTestString, arena, 0, nullptr, &error);
		CheckAndFreeToken(result, { " = oytoyt" }, arena);
		CHECK(error.ErrorCode == ErrorCodes::QuoteDidNotEnd);

		result = ParseToken(testString, arena, 86);
		CheckAndFreeToken(result, { "fdgfr" }, arena);
	}

	SUBCASE("ParsePair")
	{
		int32 offset = 0;
		auto result = ParsePair(testString, arena, 0, &offset);
		CheckAndFreePair(result, { {"a"}, {"b"} }, arena);

		result = ParsePair(testString, arena, offset, &offset);
		CheckAndFreePair(result, { {"a"}, {"d"} }, arena);

		result = ParsePair(testString, arena, offset, &offset);
		CheckAndFreePair(result, { {"cdef"}, {"abcd = ds"} }, arena);

		result = ParsePair(testString, arena, offset, &offset);
		CheckAndFreePair(result, { {"ref=\\\""}, {"123"} }, arena);

		ParseError error = {};
		result = ParsePair(testString, arena, offset, &offset, &error);
		CheckAndFreePair(result, { {"def"}, {} }, arena);
		CHECK(error.ErrorCode == ErrorCodes::NoEqualsBeforeNextToken);

		HMString errorTestString = { "\" = oytoyt" };
		result = ParsePair(errorTestString, arena, 0, nullptr, &error);
		CheckAndFreePair(result, { {" = oytoyt"}, {} }, arena);
		CHECK(error.ErrorCode == ErrorCodes::QuoteDidNotEnd);

		result = ParsePair(testString, arena, 86, nullptr);
		CheckAndFreePair(result, { {"fdgfr"}, {"#gdddddg"} }, arena);
	}

	SUBCASE("ParseText")
	{
		int32 offset = 0;
		auto result = ParseText({ "a = b" }, arena);
		CheckDictSet({ "a" }, { "b" }, result);

		result = ParseText({testData, 46}, arena);
		CheckDictSet({ "a" }, { "d" }, result);
		CheckDictSet({ "cdef" }, { "abcd = ds" }, result);
		CheckDictSet({ "ref=\\\"" }, { "123" }, result);

		result = ParseText({ testData + 65 }, arena); 
		CheckDictSet({ "def dsd" }, { "oytoyt" }, result);
		CheckDictSet({ "fdgfr" }, { "#gdddddg" }, result);

		StaticArray<ParseError> errors = {};
		result = ParseText(testString, arena, 0, &errors);
		ParseError expectedError = { ErrorCodes::NoEqualsBeforeNextToken, 4, 4 };
		CHECK(errors[0] == expectedError);
		CheckDictSet({ "a" }, { "d" }, result);
		CheckDictSet({ "def" }, {}, result);
		CheckDictSet({ "dsd" }, { "oytoyt" }, result);
		CheckDictSet({ "fdgfr" }, { "#gdddddg" }, result);
	}
}

#endif //HANDMADE_TEST_PAIRTOKENIZER