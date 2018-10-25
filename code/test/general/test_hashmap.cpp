#ifndef HANDMADE_TEST_HASHMAP
#define HANDMADE_TEST_HASHMAP

#include "../../general/HashMap.h"
#include "../../general/memory/NewDeleteArena.h"

TEST_CASE("Testing HashMap")
{
	using Collections::HashMap;

	Memory::NewDeleteArena arena = Memory::NewDeleteArena();
	auto map = HashMap<HMString*, int32, Memory::NewDeleteArena>(&arena);

	SUBCASE("Add")
	{
		char *text = "brwaesdas";
		HMString test = HMString(text, 10);
		map.Add(&test, 3);
		CHECK(map.Capacity() == 4);
		CHECK(map.Length() == 1);
		CHECK(map[&test] == 3);

		map.Add(&test, 2);
		CHECK(map.Capacity() == 4);
		CHECK(map.Length() == 1);
		CHECK(map[&test] == 2);

		char *text2 = "mofumofu";
		HMString test2 = HMString(text2, 9);
		map.Add(&test2, 5);
		CHECK(map.Capacity() == 4);
		CHECK(map.Length() == 2);
		CHECK(map[&test2] == 5);

		char *text3 = "mofumofub";
		HMString test3 = HMString(text3, 10);
		map.Add(&test3, 700);
		CHECK(map.Capacity() == 4);
		CHECK(map.Length() == 3);
		CHECK(map[&test3] == 700);

		char *text4 = "mrgrgr";
		HMString test4 = HMString(text4, 7);
		map.Add(&test4, 6);
		CHECK(map.Capacity() == 4);
		CHECK(map.Length() == 4);
		CHECK(map[&test4] == 6);

		char *text5 = "mrgrgrb";
		HMString test5 = HMString(text5, 8);
		map.Add(&test5, 20);
		CHECK(map.Capacity() == 8);
		CHECK(map.Length() == 5);
		CHECK(map[&test5] == 20);

		map.Add(&test4, 17);
		CHECK(map.Capacity() == 8);
		CHECK(map.Length() == 5);
		CHECK(map[&test4] == 17);

		HMString test6 = { "test6" };
		HMString test7 = { "test7" };
		HMString test8 = { "test8" };
		HMString test9 = { "test9" };
		map.Add(&test6, 21);
		map.Add(&test7, 22);
		map.Add(&test8, 23);
		map.Add(&test9, 24);
		CHECK(map.Capacity() == 16);
		CHECK(map.Length() == 9);
	}

	SUBCASE("CheckExists")
	{
		char *text = "brwaesdas";
		HMString test = HMString(text, 10);
		map.Add(&test, 3);
		CHECK(map.CheckExists(&test));

		map.Add(&test, 2);
		CHECK(map.CheckExists(&test));

		char *text2 = "mofumofu";
		HMString test2 = HMString(text2, 9);
		map.Add(&test2, 5);
		CHECK(map.CheckExists(&test2));

		char *text3 = "mofumofub";
		HMString test3 = HMString(text3, 10);
		CHECK(!map.CheckExists(&test3));
	}

	SUBCASE("Array Access")
	{
		char *text = "brwaesdas";
		HMString test = HMString(text, 10);
		map.Add(&test, 3);
		CHECK(map[&test] == 3);

		map[&test] = 4;
		CHECK(map[&test] == 4);

		char *text2 = "mofumofu";
		HMString test2 = HMString(text2, 9);
		map.Add(&test2, 5);

		char *text3 = "mofumofub";
		HMString test3 = HMString(text3, 10);
		map.Add(&test3, 6);

		map[&test] = 7;
		CHECK(map[&test] == 7);
		CHECK(map[&test2] == 5);
		map[&test2] = 10;
		CHECK(map[&test] == 7);
		CHECK(map[&test2] == 10);
		CHECK(map[&test3] == 6);
	}

	SUBCASE("Remove")
	{
		char *text = "brwaesdas";
		HMString test = HMString(text, 10);
		map.Add(&test, 3);

		map.Remove(&test);
		CHECK(!map.CheckExists(&test));
		CHECK(map.Length() == 0);
		CHECK(map.Capacity() == 4);

		char *text2 = "mofumofu";
		HMString test2 = HMString(text2, 9);
		map.Add(&test2, 5);

		char *text3 = "mofumofub";
		HMString test3 = HMString(text3, 10);
		map.Add(&test3, 6);

		map.Remove(&test2);
		CHECK(!map.CheckExists(&test2));
		CHECK(map.CheckExists(&test3));
		CHECK(map.Length() == 1);
		CHECK(map.Capacity() == 4);

		map.Remove(&test);
		CHECK(map.Length() == 1);
		CHECK(map.Capacity() == 4);
	}
}

#endif