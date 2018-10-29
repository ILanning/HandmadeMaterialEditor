#ifndef HANDMADE_TEST_STRINGHELPERS
#define HANDMADE_TEST_STRINGHELPERS

#include "../../handmade_typedefs.h"
#include "../../general/StringHelpers.cpp"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../libraries/doctest.h"
#endif

TEST_CASE("Testing various C-string helper functions")
{
	SUBCASE("Testing IsEqual")
	{
		char *stringA = "Testing 1";
		char *stringB = "Testing 1";
		char *stringC = "Testing 2";
		char *emptyStringA = "";
		char *emptyStringB = "";
		char *nullString = nullptr;
		char *nonNullTermA = "Test part2\0 test";
		char *nonNullTermB = "Test part2\0 test";
		char *nonNullTermC = "Test part2\0 tes3";
		
		CHECK(CString::IsEqual(stringA, stringB));
		CHECK(!CString::IsEqual(stringA, stringC));
		CHECK(CString::IsEqual(nullString, nullString));
		CHECK(!CString::IsEqual(nullString, stringA));
		CHECK(CString::IsEqual(emptyStringA, emptyStringB));
		
		CHECK(CString::IsEqual(stringA, stringC, 8, 8));
		CHECK(!CString::IsEqual(stringA, stringC, 10, 10));
		CHECK(CString::IsEqual(nullString, nullString, 4, 4));
		CHECK(!CString::IsEqual(nullString, stringA, 3, 3));
		CHECK(CString::IsEqual(emptyStringA, emptyStringB, 1, 1));
		CHECK(CString::IsEqual(nonNullTermA, nonNullTermB, 17, 17));
		CHECK(CString::IsEqual(nonNullTermA, nonNullTermA, 4, 16, 3, 15));
	}
	SUBCASE("Testing IsDigit")
	{
		CHECK(!CString::IsDigit('0' - 1));
		CHECK(CString::IsDigit('0'));
		CHECK(CString::IsDigit('9'));
		CHECK(!CString::IsDigit('9' + 1));

		CHECK(!CString::IsDigit('a'));
		CHECK(!CString::IsDigit('\0'));
	}
	SUBCASE("Testing IsLetter")
	{
		CHECK(!CString::IsLetter('2'));

		CHECK(!CString::IsLetter('a' - 1));
		CHECK(CString::IsLetter('a'));
		CHECK(CString::IsLetter('z'));
		CHECK(!CString::IsLetter('z' + 1));

		CHECK(!CString::IsLetter('A' - 1));
		CHECK(CString::IsLetter('A'));
		CHECK(CString::IsLetter('Z'));
		CHECK(!CString::IsLetter('Z' + 1));

		CHECK(!CString::IsLetter('\0'));
	}
	SUBCASE("Testing IsAlphanumeric")
	{
		CHECK(!CString::IsAlphanumeric('0' - 1));
		CHECK(CString::IsAlphanumeric('0'));
		CHECK(CString::IsAlphanumeric('9'));
		CHECK(!CString::IsAlphanumeric('9' + 1));

		CHECK(!CString::IsAlphanumeric('a' - 1));
		CHECK(CString::IsAlphanumeric('a'));
		CHECK(CString::IsAlphanumeric('z'));
		CHECK(!CString::IsAlphanumeric('z' + 1));

		CHECK(!CString::IsAlphanumeric('A' - 1));
		CHECK(CString::IsAlphanumeric('A'));
		CHECK(CString::IsAlphanumeric('Z'));
		CHECK(!CString::IsAlphanumeric('Z' + 1));

		CHECK(!CString::IsAlphanumeric('\0'));
	}
	SUBCASE("Testing GetLength")
	{
		char *testString = "Testing testing mkg213";
		char *emptyString = "";

		CHECK(CString::GetLength(testString) == 23);
		CHECK(CString::GetLength(emptyString) == 1);
	}
	SUBCASE("Testing FindCharacter single character")
	{
		char *testString = "Testing testing mkg213";
		char *emptyString = "";

		CHECK(CString::FindCharacter(testString, 'T', 23) == 0);
		CHECK(CString::FindCharacter(testString, 'i', 23) == 4);
		CHECK(CString::FindCharacter(testString, '5', 23) == -1);
		CHECK(CString::FindCharacter(testString, 'i') == 4);
		CHECK(CString::FindCharacter(testString, '5') == -1);

		CHECK(CString::FindCharacter(emptyString, '5', 0) == -1);
		CHECK(CString::FindCharacter(emptyString, '\0', 1) == 0);
		CHECK(CString::FindCharacter(emptyString, '5') == -1);
	}
	SUBCASE("Testing FindCharacter multiple characters")
	{
		char *testString = "Testing testing mkg213";
		char *emptyString = "";

		CHECK(CString::FindCharacter(testString, "m2", 2, 23) == 16);
		CHECK(CString::FindCharacter(testString, "vbr", 3, 23) == -1);
		CHECK(CString::FindCharacter(testString, "m2", 2) == 16);
		CHECK(CString::FindCharacter(testString, "vbr", 3) == -1);

		CHECK(CString::FindCharacter(emptyString, "5b", 2, 0) == -1);
		CHECK(CString::FindCharacter(emptyString, "\0l", 2, 1) == 0);
		CHECK(CString::FindCharacter(emptyString, "5b", 2) == -1);
	}
	SUBCASE("Testing FindLastCharacter single character")
	{
		char *testString = "Testing testing mkg213";
		char *emptyString = "";

		CHECK(CString::FindLastCharacter(testString, 'T', 23) == 0);
		CHECK(CString::FindLastCharacter(testString, 'i', 23) == 12);
		CHECK(CString::FindLastCharacter(testString, '5', 23) == -1);
		CHECK(CString::FindLastCharacter(testString, 'i') == 12);
		CHECK(CString::FindLastCharacter(testString, '5') == -1);

		CHECK(CString::FindLastCharacter(emptyString, '5', 0) == -1);
		CHECK(CString::FindLastCharacter(emptyString, '\0', 1) == 0);
		CHECK(CString::FindLastCharacter(emptyString, '5') == -1);
	}
	SUBCASE("Testing FindLastCharacter multiple characters")
	{
		char *testString = "Testing testing mkg213";
		char *emptyString = "";

		CHECK(CString::FindLastCharacter(testString, "m2", 2, 23) == 19);
		CHECK(CString::FindLastCharacter(testString, "vbr", 3, 23) == -1);
		CHECK(CString::FindLastCharacter(testString, "m2", 2) == 19);
		CHECK(CString::FindLastCharacter(testString, "vbr", 3) == -1);

		CHECK(CString::FindLastCharacter(emptyString, "5b", 2, 0) == -1);
		CHECK(CString::FindLastCharacter(emptyString, "\0l", 2, 1) == 0);
		CHECK(CString::FindLastCharacter(emptyString, "5b", 2) == -1);
	}
	SUBCASE("Testing FindNonWhitespace")
	{
		char *testStringA = "Testing mkg213 ";
		char *testStringB = " \t\r\n\f\v";
		char *emptyString = "";

		CHECK(CString::FindNonWhitespace(testStringA) == 0);
		CHECK(CString::FindNonWhitespace(testStringB) == -1);
		CHECK(CString::FindNonWhitespace(testStringA, 16, 7) == 8);

		CHECK(CString::FindNonWhitespace(emptyString) == -1);
		CHECK(CString::FindNonWhitespace(emptyString, 1) == 0);
	}
	SUBCASE("Testing FindLineEnd")
	{
		char *testStringA = "Testi\r\ng mkg213 \n";
		char *testStringB = " fgdgfd gff";
		char *emptyString = "";

		CHECK(CString::FindLineEnd(testStringA) == 5);
		CHECK(CString::FindLineEnd(testStringB) == -1);
		CHECK(CString::FindLineEnd(testStringA, 18, 7) == 16);

		CHECK(CString::FindLineEnd(emptyString) == -1);
		CHECK(CString::FindLineEnd(emptyString, 1) == -1);
	}
	SUBCASE("Testing FindSubstring")
	{
		char *searchA = "tes";
		char *withinA = "test\n\t te Searchtes";

		CHECK(CString::FindSubstring(searchA, 3, withinA, 20) == 0);
		CHECK(CString::FindSubstring(searchA, 3, withinA, 20, 2) == 16);
	}
	SUBCASE("Testing CountSubstrings")
	{

	}
	SUBCASE("Testing CopySubstring")
	{

	}
	SUBCASE("Testing UpperChar")
	{
		CHECK(CString::UpperChar('u') == 'U');
		CHECK(CString::UpperChar('U') == 'U');
		CHECK(CString::UpperChar('!') == '!');
	}
	SUBCASE("Testing LowerChar")
	{
		CHECK(CString::LowerChar('U') == 'u');
		CHECK(CString::LowerChar('u') == 'u');
		CHECK(CString::LowerChar('!') == '!');
	}
	SUBCASE("Testing EditToUpper")
	{
		char *testString = "TestString 123\n";
		char *testResult = "TESTSTRING 123\n";

		char *modified = CString::CopySubstring(testString, 15);
		CString::EditToUpper(modified, 16, 0, 16);

		CHECK(CString::IsEqual(modified, testResult));

		delete[] modified;
	}
	SUBCASE("Testing EditToLower")
	{
		char *testString = "TestString 123\n";
		char *testResult = "teststring 123\n";

		char *modified = CString::CopySubstring(testString, 15);
		CString::EditToLower(modified, 16, 0, 16);

		CHECK(CString::IsEqual(modified, testResult));

		delete[] modified;
	}
}

#endif