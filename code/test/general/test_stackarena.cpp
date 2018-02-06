#ifndef HANDMADE_TEST_STACKARENA
#define HANDMADE_TEST_STACKARENA


#include "../../general/memory/StackArena.h"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libraries/doctest.h"
#endif

TEST_CASE("Testing StackArena")
{
	uint32 arenaSize = Megabytes(2);
	Memory::StackArena *arena = new Memory::StackArena(new uint8[arenaSize], arenaSize);

	REQUIRE(arena->buffer != nullptr);
	REQUIRE(arena->size == arenaSize);
	REQUIRE(arena->bufferEnd == arena->buffer + arenaSize);
	REQUIRE((arena->bufferNext >= arena->buffer && arena->bufferNext < arena->bufferEnd));

	SUBCASE("Adding items")
	{
		uint32 *ints = (uint32 *)arena->Allocate(sizeof(uint32) * 4);
		CHECK(arena->bufferNext - arena->buffer == sizeof(uint32) * 4 + sizeof(uint32));

		uint16 *shorts = (uint16 *)arena->Allocate(sizeof(uint16) * 9);
		CHECK((uint64)shorts - (uint64)ints == sizeof(uint32) * 4 + sizeof(uint32));
		CHECK((uint64)arena->bufferNext - (uint64)shorts == sizeof(uint16) * 9 + sizeof(uint32));

		uint8 *bytes = (uint8 *)arena->Allocate(sizeof(uint8) * 1163);
		CHECK((uint64)bytes - (uint64)shorts == sizeof(uint16) * 9 + sizeof(uint32));
		CHECK((uint64)arena->bufferNext - (uint64)bytes == sizeof(uint8) * 1163 + sizeof(uint32));
	}

	SUBCASE("Removing items")
	{
		uint32 *ints = (uint32 *)arena->Allocate(sizeof(uint32) * 4);
		uint16 *shorts = (uint16 *)arena->Allocate(sizeof(uint16) * 9);
		uint8 *bytes = (uint8 *)arena->Allocate(sizeof(uint8) * 1163);
		arena->DeallocateLast();
		CHECK(arena->bufferNext == bytes);
	}

	SUBCASE("Heavy use")
	{

	}

	SUBCASE("Destruction")
	{

	}
}

#endif // !HANDMADE_TEST_STACKARENA
