#ifndef HANDMADE_TEST_FIXEDSIZEARENA
#define HANDMADE_TEST_FIXEDSIZEARENA


#include "../../general/memory/FixedSizeArena.h"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libraries/doctest.h"
#endif

namespace TestFixedSizeArenaHelpers
{
	struct TestStruct
	{
		int32 testMemberA = 0;
		uint16 testMemberB = 0;
		uint16 testMemberC = 0;
		uint16 testMemberD = 0;
	};

	static void Allocate(Memory::FixedSizeArena *arena)
	{
		TestStruct *structPtrs[4] = {};
		structPtrs[0] = (TestStruct *)arena->Allocate();
		CHECK(structPtrs[0] != nullptr);
		CHECK(arena->bufferNext == arena->buffer + sizeof(TestStruct));

		structPtrs[1] = (TestStruct *)arena->Allocate(5);
		CHECK(structPtrs[1] != nullptr);
		CHECK(arena->bufferNext == (uint8 *)(structPtrs[1] + 1));

		structPtrs[2] = (TestStruct *)arena->Allocate();
		structPtrs[3] = (TestStruct *)arena->Allocate();
		CHECK(arena->Allocate() == nullptr);
	}

	static void Deallocate(Memory::FixedSizeArena *arena)
	{
		TestStruct *structPtrs[4] = {};
		structPtrs[0] = (TestStruct *)arena->Allocate();
		CHECK((uint8 *)(arena->deletedListEnd) == arena->bufferEnd);
		CHECK(arena->Deallocate(structPtrs[0]));
		CHECK((uint8 *)(arena->deletedListEnd) == arena->bufferEnd);
		CHECK(arena->bufferNext == arena->buffer);

		structPtrs[0] = (TestStruct *)arena->Allocate();
		structPtrs[1] = (TestStruct *)arena->Allocate();
		CHECK(arena->Deallocate(structPtrs[0]));
		CHECK((uint8 *)(arena->deletedListEnd) == arena->bufferEnd - sizeof(uint8 *));
		CHECK(arena->bufferNext == (uint8 *)structPtrs[1] + sizeof(TestStruct));

		structPtrs[2] = (TestStruct *)arena->Allocate();
		CHECK(structPtrs[0] == structPtrs[2]);
		CHECK((uint8 *)(arena->deletedListEnd) == arena->bufferEnd);
		CHECK(arena->bufferNext == (uint8 *)structPtrs[1] + sizeof(TestStruct));
	}

	void ContainedBlock()
	{
		uint8 *testBuffer = new uint8[150];
		Memory::FixedSizeArena *test = Memory::FixedSizeArena::CreateContainedBlock(testBuffer, 150, sizeof(TestStruct));

		CHECK(test->buffer == testBuffer + sizeof(Memory::FixedSizeArena));

		test->~FixedSizeArena();

		CHECK(test->buffer == nullptr);

		delete testBuffer;
	}
}

TEST_CASE("Testing FixedSizeArena")
{
	uint32 arenaSize = Megabytes(2);
	int32 testStrSize = sizeof(TestFixedSizeArenaHelpers::TestStruct);
	SUBCASE("Allocating") 
	{
		uint8 *mem = new uint8[testStrSize * 4];
		Memory::FixedSizeArena *arena = new Memory::FixedSizeArena(mem, testStrSize * 4, testStrSize);
		TestFixedSizeArenaHelpers::Allocate(arena);

		delete[] mem;
		delete arena;
	}
	SUBCASE("Removing")
	{
		uint8 *mem = new uint8[arenaSize];
		Memory::FixedSizeArena *arena = new Memory::FixedSizeArena(mem, arenaSize, testStrSize);

		REQUIRE(arena->buffer != nullptr);
		REQUIRE(arena->size == arenaSize);
		REQUIRE(arena->bufferEnd == arena->buffer + arenaSize);
		REQUIRE((arena->bufferNext >= arena->buffer && arena->bufferNext < arena->bufferEnd));
		TestFixedSizeArenaHelpers::Deallocate(arena);

		delete[] mem;
		delete arena;
	}
	SUBCASE("Contained block")
	{
		TestFixedSizeArenaHelpers::ContainedBlock();
	}

	SUBCASE("Owns own memory")
	{
		Memory::FixedSizeArena *arena = new Memory::FixedSizeArena(arenaSize, testStrSize, TestAlloc, TestDealloc);

		REQUIRE(arena->buffer != nullptr);
		REQUIRE(arena->GetRemainingSlots() == arenaSize / testStrSize);
		REQUIRE(arena->bufferEnd == arena->buffer + arenaSize);
		REQUIRE((arena->bufferNext >= arena->buffer && arena->bufferNext < arena->bufferEnd));

		TestFixedSizeArenaHelpers::Deallocate(arena);

		delete arena;
	}
}

#endif //!HANDMADE_FIXEDSIZEARENA_H