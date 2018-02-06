#ifndef HANDMADE_TEST_FIXEDSIZEARENA
#define HANDMADE_TEST_FIXEDSIZEARENA


#include "../../general/memory/FixedSizeArena.h"

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../libraries/doctest.h"
#endif

struct testStruct
{
	int32 testMemberA = 0;
	uint16 testMemberB = 0;
	char testMemberC = '\0';
};

TEST_CASE("Testing FixedSizeArena")
{
	uint32 arenaSize = Megabytes(2);
	Memory::FixedSizeArena *arena = new Memory::FixedSizeArena(new uint8[arenaSize], arenaSize, 7);

	REQUIRE(arena->buffer != nullptr);
	REQUIRE(arena->size == arenaSize);
	REQUIRE(arena->bufferEnd == arena->buffer + arenaSize);
	REQUIRE((arena->bufferNext >= arena->buffer && arena->bufferNext < arena->bufferEnd));

	SUBCASE("Allocating") 
	{
		item = arena->Allocate();
	}
	SUBCASE("Removing")
	{
		testStruct *testStructArray[11];
		arena->Allocate();
	}
	SUBCASE("Contained block")
	{

	}
}

#endif //!HANDMADE_FIXEDSIZEARENA_H