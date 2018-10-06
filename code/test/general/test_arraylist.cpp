#ifndef HANDMADE_TEST_ARRAYLIST
#define HANDMADE_TEST_ARRAYLIST

#include "../../general/ArrayList.h"
#include "../../general/memory/NewDeleteArena.h"

struct TestStruct
{

};

TEST_CASE("Testing ArrayList")
{
	const int32 listStartSize = 30;
	Memory::NewDeleteArena testArena = Memory::NewDeleteArena();
	auto list = ArrayList<int32, Memory::NewDeleteArena>(&testArena, listStartSize);
	REQUIRE(list.Capacity() == listStartSize);

	auto emptyList = ArrayList<int32, Memory::NewDeleteArena>(&testArena, 0);
	REQUIRE(emptyList.Capacity() == 0);

	SUBCASE("Add")
	{
		list.Add(5);
		CHECK(list[0] == 5);
		CHECK(list.Count() == 1);
		CHECK(list.Capacity() == listStartSize);

		for (int i = 1; i < 30; i++)
		{
			list.Add(i);
		}
		CHECK(list[29] == 29);
		CHECK(list.Count() == 30);
		CHECK(list.Capacity() == 30);

		list.Add(30);
		CHECK(list[0] == 5);
		CHECK(list[29] == 29);
		CHECK(list[30] == 30);
		CHECK(list.Count() == 31);
		CHECK(list.Capacity() == listStartSize * list.UpsizeFactor);
	}

	SUBCASE("AddRange")
	{
		int32 testArray1[] = {
			4, 7, 12, -5, 2
		};
		list.AddRange(testArray1, 5);

		CHECK(list[0] == 4);
		CHECK(list[2] == 12);
		CHECK(list[4] == 2);
		CHECK(list.Count() == 5);
		CHECK(list.Capacity() == listStartSize);

		emptyList.AddRange(testArray1, 5);

		CHECK(emptyList[0] == 4);
		CHECK(emptyList[4] == 2);
		CHECK(emptyList.Count() == 5);
		CHECK(emptyList.Capacity() == 5);

		int32 testArray2[125];
		testArray2[0] = 100;
		testArray2[70] = 200;
		testArray2[124] = 300;
		list.AddRange(testArray2, 125);

		CHECK(list[5] == 100);
		CHECK(list[75] == 200);
		CHECK(list[129] == 300);
		CHECK(list.Count() == 130);
		CHECK(list.Capacity() == (int32)(listStartSize * list.UpsizeFactor * list.UpsizeFactor * list.UpsizeFactor));
	}

	SUBCASE("Insert")
	{
		list.Add(2);
		list.Add(4);
		list.Insert(3, 1);
		CHECK(list[1] == 3);
		CHECK(list.Count() == 3);
		CHECK(list.Capacity() == listStartSize);

		int32 testArray[27];
		testArray[17] = 21;
		testArray[25] = 29;
		testArray[26] = 30;
		list.AddRange(testArray, 27);
		list.Insert(100, 20);

		CHECK(list[20] == 100);
		CHECK(list[21] == 21);
		CHECK(list[29] == 29);
		CHECK(list[30] == 30);

		list.Insert(45, 45);
		CHECK(list[45] == 45);
		CHECK(list.Capacity() == (int32)(listStartSize * list.UpsizeFactor));

		emptyList.Insert(6, 6);
		CHECK(emptyList[6] == 6);
		CHECK(emptyList.Capacity() == 6);
	}

	SUBCASE("InsertRange")
	{
		int32 testArray1[] = { 1, 2, 3 };
		list.Add(0);
		list.Add(4);
		list.InsertRange(testArray1, 1, 3);
		CHECK(list[1] == 1);
		CHECK(list[3] == 3);
		CHECK(list[4] == 4);
		CHECK(list.Count() == 5);
		CHECK(list.Capacity() == listStartSize);

		int32 testArray2[30];
		testArray2[0] = 10;
		testArray2[29] = 20;
		list.InsertRange(testArray2, 2, 30);
		CHECK(list[2] == 10);
		CHECK(list[31] == 20);
		CHECK(list[32] == 2);
		CHECK(list[34] == 4);
		CHECK(list.Count() == 35);
		CHECK(list.Capacity() == (int32)(listStartSize * list.UpsizeFactor));

		emptyList.InsertRange(testArray1, 6, 3);
		CHECK(emptyList[6] == 1);
		CHECK(emptyList[8] == 3);
		CHECK(emptyList.Capacity() == 9);
	}

	SUBCASE("Delete")
	{
		list.Add(5);
		list.Delete(0);
		CHECK(list.Capacity() == listStartSize);
		CHECK(list.Count() == 0);

		list.Add(5);
		list.Add(6);
		list.Add(7);
		list.Delete(1);
		CHECK(list[0] == 5);
		CHECK(list[1] == 7);
		CHECK(list.Capacity() == listStartSize);
		CHECK(list.Count() == 2);

		list.DownsizeFactor = 0.4f;
		list.Add(8);
		list.Delete(1);
		CHECK(list[0] == 5);
		CHECK(list[1] == 8);
		CHECK(list.Capacity() == 4);
		CHECK(list.Count() == 2);

		list.Delete(5);
		emptyList.Delete(0);
		CHECK(list.Count() == 2);
		CHECK(emptyList.Count() == 0);
	}

	SUBCASE("DeleteRange")
	{
		int32 testArray1[] = { 0,1,2,3,4,5,6,7,8,9 };
		list.AddRange(testArray1, 10);
		list.DeleteRange(3, 5);
		CHECK(list[0] == 0);
		CHECK(list[2] == 2);
		CHECK(list[3] == 8);
		CHECK(list[4] == 9);
		CHECK(list.Capacity() == listStartSize);
		CHECK(list.Count() == 5);
		
		list.DownsizeFactor = 0.4f;
		list.DeleteRange(3, 5);
		CHECK(list[0] == 0);
		CHECK(list[2] == 2);
		CHECK(list.Capacity() == 4);
		CHECK(list.Count() == 3);

		emptyList.DeleteRange(6, 10);
		CHECK(emptyList.Capacity() == 0);
		CHECK(emptyList.Count() == 0);
	}

	SUBCASE("RangeToArray")
	{
		int32 testArray1[] = { 0,1,2,3,4,5,6,7,8,9 };
		list.AddRange(testArray1, 10);
		int32 *testOutput = list.RangeToArray(&testArena, 3, 5);
		CHECK(testOutput[0] == 3);
		CHECK(testOutput[2] == 5);
		CHECK(testOutput[4] == 7);
		testArena.Deallocate(testOutput);

		testOutput = list.RangeToArray(&testArena, 0, 0);
		CHECK(testOutput == nullptr);

		testOutput = emptyList.RangeToArray(&testArena, 0, 0);
		CHECK(testOutput == nullptr);
	}

	SUBCASE("ToArray")
	{

		int32 testArray1[] = { 0,1,2,3,4,5,6,7,8,9 };
		list.AddRange(testArray1, 10);
		int32 *testOutput = list.ToArray(&testArena);
		CHECK(testOutput[0] == 0);
		CHECK(testOutput[5] == 5);
		CHECK(testOutput[9] == 9);
		testArena.Deallocate(testOutput);

		list.DeleteRange(0, 10);
		testOutput = list.ToArray(&testArena);
		CHECK(testOutput == nullptr);

		testOutput = emptyList.ToArray(&testArena);
		CHECK(testOutput == nullptr);
	}
}
#endif