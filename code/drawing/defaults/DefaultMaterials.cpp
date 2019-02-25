#ifndef HANDMADE_DEFAULTMATERIALS_CPP
#define HANDMADE_DEFAULTMATERIALS_CPP

#include "../../handmade_typedefs.h"
#include "../../content/AssetPtr.h"
#include "../../content/AssetPtrSharedData.h"
#include "../../general/StringHelpers.h"
#include "../Texture2D.h"
#include "../Material.h"

namespace Drawing
{
	namespace Defaults
	{
		AssetPtr<Texture2D> BlankTexture = { new Texture2D(new uint8[4]{ 255, 255, 255, 255 }, 1, 1, GL_RGBA, GL_RGBA, GL_REPEAT), new Content::AssetPtrSharedData() };
		AssetPtr<Material> BlankMaterial = { new Material(BlankTexture), new Content::AssetPtrSharedData() };
	}
}

#endif //HANDMADE_DEFAULTMATERIALS_CPP