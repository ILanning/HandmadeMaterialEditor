#ifndef HANDMADE_MESHCOLLECTION_H
#define HANDMADE_MESHCOLLECTION_H

#include "../drawing/Mesh.h"
#include "../general/ArrayList.h"
#include "../general/memory/NewDeleteArena.h"

namespace Content
{
	typedef	Collections::ArrayList<Drawing::Mesh, Memory::NewDeleteArena> MeshCollection;
}

#endif //HANDMADE_MESHCOLLECTION_H