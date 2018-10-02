#ifndef HANDMADE_TEST_STACKARENA
#define HANDMADE_TEST_STACKARENA


#include "../../general/memory/StackArena.h"
#include "../test_helpers.cpp"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libraries/doctest.h"
#endif
namespace TestStackArenaHelpers
{
	static void AddingItems(Memory::StackArena *arena)
	{
		uint32 *ints = (uint32 *)arena->Allocate(sizeof(uint32) * 4);
		CHECK(arena->bufferNext - arena->buffer == sizeof(uint32) * 4 + sizeof(uint64));

		uint16 *shorts = (uint16 *)arena->Allocate(sizeof(uint16) * 9);
		CHECK((uint64)shorts - (uint64)ints == sizeof(uint32) * 4 + sizeof(uint64));
		CHECK((uint64)arena->bufferNext - (uint64)shorts == sizeof(uint16) * 9 + sizeof(uint64));

		uint8 *bytes = (uint8 *)arena->Allocate(sizeof(uint8) * 1163);
		CHECK((uint64)bytes - (uint64)shorts == sizeof(uint16) * 9 + sizeof(uint64));
		CHECK((uint64)arena->bufferNext - (uint64)bytes == sizeof(uint8) * 1163 + sizeof(uint64));
	}

	static void RemovingItems(Memory::StackArena *arena)
	{
		uint32 *ints = (uint32 *)arena->Allocate(sizeof(uint32) * 4);
		uint16 *shorts = (uint16 *)arena->Allocate(sizeof(uint16) * 9);
		uint8 *bytes = (uint8 *)arena->Allocate(sizeof(uint8) * 1163);
		CHECK(arena->DeallocateLast());
		CHECK(arena->bufferNext == bytes);
		char *someString = (char *)arena->Allocate(sizeof(char) * 50);
		CHECK(arena->bufferNext == (uint8 *)(someString + sizeof(char) * 50 + sizeof(uint64)));
		CHECK_FALSE(arena->Deallocate(ints)); //Trying to pop from somewhere other than the front of the stack
		CHECK(arena->Deallocate(someString)); //Popping the last item by pointer value
		arena->DeallocateLast();
		arena->DeallocateLast();
		CHECK_FALSE(arena->DeallocateLast());  //Popping from an empty stack
	}

	static void PartialAlloc(Memory::StackArena *arena, uint64 arenaSize)
	{
		CHECK(arena->GetRemainingBytes() == arenaSize);


		char *checkBasicUse = (char *)arena->PartialAlloc(50);

		REQUIRE(checkBasicUse != nullptr);
		CHECK(arena->partialInProgress == true);
		CHECK(arena->GetRemainingBytes() == arenaSize - 50);
		CHECK(arena->GetPartialAllocCurrentSize() == 50);

		for (uint8 i = 1; i < 50; i++)
		{
			*checkBasicUse = (char)i;
			checkBasicUse++;
		}
		*checkBasicUse = '\0';
		checkBasicUse++;

		char *grownPartial = (char *)arena->PartialAlloc(1000);
		
		REQUIRE(grownPartial != nullptr);
		CHECK(grownPartial == checkBasicUse);
		CHECK(arena->partialInProgress == true);
		CHECK(arena->GetRemainingBytes() == arenaSize - 1050);
		CHECK(arena->GetPartialAllocCurrentSize() == 1050);

		arena->ClosePartialAlloc();

		CHECK(arena->partialInProgress == false);
		CHECK(arena->GetPartialAllocCurrentSize() == 0);


		char *spacerAlloc = (char *)arena->Allocate(2048);
		char *checkCancel = (char *)arena->PartialAlloc(256);

		REQUIRE(checkCancel != nullptr);
		CHECK(arena->partialInProgress == true);
		CHECK(arena->GetRemainingBytes() == arenaSize - 1050 - 2048 - 256 - (sizeof(uint64) * 2));
		CHECK(arena->GetPartialAllocCurrentSize() == 256);

		arena->CancelPartialAlloc();
		
		CHECK(arena->partialInProgress == false);
		CHECK(arena->GetPartialAllocCurrentSize() == 0);
		CHECK((char *)arena->bufferNext == checkCancel);


		char *checkShrinkPartial = (char *)arena->PartialAlloc(256);

		REQUIRE(checkShrinkPartial != nullptr);
		CHECK(arena->partialInProgress == true);
		CHECK(arena->GetRemainingBytes() == arenaSize - 1050 - 2048 - 256 - (sizeof(uint64) * 2));
		CHECK(arena->GetPartialAllocCurrentSize() == 256);

		arena->ReducePartialAllocBy(23);

		CHECK(arena->GetRemainingBytes() == arenaSize - 1050 - 2048 - 233 - (sizeof(uint64) * 2));
		CHECK(arena->GetPartialAllocCurrentSize() == 233);

		arena->ReducePartialAllocTo(200);

		CHECK(arena->GetRemainingBytes() == arenaSize - 1050 - 2048 - 200 - (sizeof(uint64) * 2));
		CHECK(arena->GetPartialAllocCurrentSize() == 200);

		arena->ClosePartialAlloc();

		CHECK(arena->partialInProgress == false);
		CHECK(arena->GetPartialAllocCurrentSize() == 0);
		CHECK((char *)arena->bufferNext == checkShrinkPartial + 200 + sizeof(uint64));
	}

	void ContainedBlock()
	{
		uint8 *testBuffer = new uint8[150];
		Memory::StackArena *test = Memory::StackArena::CreateContainedBlock(testBuffer, 150);

		CHECK(test->buffer == testBuffer + sizeof(Memory::StackArena));

		test->~StackArena();

		CHECK(test->buffer == nullptr);

		delete testBuffer;
	}
}

TEST_CASE("Testing StackArena")
{
	uint64 arenaSize = Megabytes(500);
	Memory::StackArena *arena;

	SUBCASE("Self-contained block")
	{

	}

	SUBCASE("Given memory")
	{
		arena = new Memory::StackArena(new uint8[arenaSize], arenaSize);

		//TODO(Ian): Doctest didn't seem to like this being moved out to a function, see if anything can be done about that
		REQUIRE(arena->buffer != nullptr);
		REQUIRE(arena->GetRemainingBytes() == arenaSize);
		REQUIRE(arena->bufferEnd == arena->buffer + arenaSize);
		REQUIRE((arena->bufferNext >= arena->buffer && arena->bufferNext < arena->bufferEnd));

		SUBCASE("Adding items")
		{
			TestStackArenaHelpers::AddingItems(arena);
		}

		SUBCASE("Removing items")
		{
			TestStackArenaHelpers::RemovingItems(arena);
		}

		SUBCASE("Partial Alloc")
		{
			TestStackArenaHelpers::PartialAlloc(arena, arenaSize);
		}

		SUBCASE("Contained Block")
		{
			TestStackArenaHelpers::ContainedBlock();
		}

		delete arena;
	}

	SUBCASE("Owns own memory")
	{
		arena = new Memory::StackArena(arenaSize, TestAlloc, TestDealloc);

		REQUIRE(arena->buffer != nullptr);
		REQUIRE(arena->GetRemainingBytes() == arenaSize);
		REQUIRE(arena->bufferEnd == arena->buffer + arenaSize);
		REQUIRE((arena->bufferNext >= arena->buffer && arena->bufferNext < arena->bufferEnd));

		SUBCASE("Adding items")
		{
			TestStackArenaHelpers::AddingItems(arena);
		}

		SUBCASE("Removing items")
		{
			TestStackArenaHelpers::RemovingItems(arena);
		}

		SUBCASE("Partial Alloc")
		{
			TestStackArenaHelpers::PartialAlloc(arena, arenaSize);
		}

		SUBCASE("Contained Block")
		{
			TestStackArenaHelpers::ContainedBlock();
		}

		delete arena;
	}
}

#endif // !HANDMADE_TEST_STACKARENA
