#ifndef HANDMADE_MTLLOADER_H
#define HANDMADE_MTLLOADER_H

#include "MTLTextureOptions.h"
#include "../handmade_typedefs.h"
#include "../drawing/Material.h"
#include "../general/ArrayList.h"
#include "../general/HMString.h"
#include "../general/HMPair.h"
#include "../general/StaticArray.h"
#include "../general/memory/NewDeleteArena.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"

namespace Content
{
	namespace OBJ
	{
		namespace _OBJInternal
		{
			Vector3 ParseMTLVec3(char *string, int32 length, int32 offset, Vector3 defaultValue, int32 *readFinishIndex);

			Vector2 ParseMTLVec2(char *string, int32 length, int32 offset, Vector2 defaultValue, int32 *readFinishIndex);

			float ParseMTLFloat(char *string, int32 length, int32 offset, float defaultValue, int32 *readFinishIndex);

			bool ParseMTLBool(char *string, int32 length, int32 offset, bool defaultValue, int32 *readFinishIndex);

			MTLTextureOptions ParseMapLineOptions(char *line, int32 lineLength, int32 offset, char *folder, bool isScalar, int32 *readFinishIndex);
		}

		typedef HMPair<HMString, Drawing::Material> NamedMTL;

		//Parses any materials in the given text and adds them to the provided collection.
		void ParseMTL(FileData toLoad, Collections::ArrayList<NamedMTL, Memory::NewDeleteArena>& materialList);

		//Returns an array containing any materials found in the given text.
		StaticArray<NamedMTL> ParseMTL(FileData file, Memory::NewDeleteArena& arena);

		//Parses any materials in the given text and adds them to the provided collection.
		void ParseMTL(HMString path, ReadFileFunc *readFile, Collections::ArrayList<NamedMTL, Memory::NewDeleteArena>& materialList);

		//Returns an array containing any materials found in the given text.
		StaticArray<NamedMTL> ParseMTL(HMString path, ReadFileFunc *readFile, Memory::NewDeleteArena& arena);
	}
}

#endif //HANDMADE_MTLLOADER_H