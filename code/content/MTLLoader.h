#ifndef HANDMADE_MTLLOADER_H
#define HANDMADE_MTLLOADER_H

#include "../handmade_typedefs.h"
#include "../general/StretchyArray.h"
#include "../drawing/Material.h"
#include "../math/Vector3.h"
#include "../math/Vector2.h"
#include "MTLTextureOptions.h"

namespace Content
{
	namespace OBJ
	{
		Vector3 ParseMTLVec3(char *string, int32 length, int32 offset, Vector3 defaultValue, int32 *readFinishIndex);

		Vector2 ParseMTLVec2(char *string, int32 length, int32 offset, Vector2 defaultValue, int32 *readFinishIndex);

		float ParseMTLFloat(char *string, int32 length, int32 offset, float defaultValue, int32 *readFinishIndex);

		bool ParseMTLBool(char *string, int32 length, int32 offset, bool defaultValue, int32 *readFinishIndex);

		MTLTextureOptions ParseMapLineOptions(char *line, int32 lineLength, int32 offset, char *folder, bool isScalar, int32 *readFinishIndex);

		StretchyArray<Drawing::Material> *ParseMTL(FileData toLoad, StretchyArray<Drawing::Material> *materialList);

		Drawing::Material *ParseMTL(FileData file, int32 &outMaterialCount);

		StretchyArray<Drawing::Material> *ParseMTL(char *path, int32 pathLength, ReadFileFunc *readFile, StretchyArray<Drawing::Material> *materialList);

		Drawing::Material *ParseMTL(char *path, int32 pathLength, ReadFileFunc *readFile, int32 &outMaterialCount);
	}
}

#endif //HANDMADE_MTLLOADER_H