#ifndef HANDMADE_MTLDICT_H
#define HANDMADE_MTLDICT_H

#include "../drawing/Material.h"
#include "../general/HashMap.h"
#include "AssetPtr.h"
#include "../general/HMString.h"
#include "../general/memory/NewDeleteArena.h"

namespace Content
{
	typedef Collections::HashMap<HMString, AssetPtr<Drawing::Material>, Memory::NewDeleteArena> MTLDict;
}

#endif //HANDMADE_MTLDICT_H