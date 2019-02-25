#ifndef HANDMADE_DEFAULTMATERIALS_H
#define HANDMADE_DEFAULTMATERIALS_H

#include "../../content/AssetPtr.h"
#include "../Texture2D.h"
#include "../Material.h"

//TODO: Probably package these up into something rather than leaving them hanging in the global namespace
//      Maybe part of a package of named defaults associated with the ContentManager?
//      ie. pass Load<Texture2D>(string) "White1x1" to get this, no file load needed
namespace Drawing
{
	namespace Defaults
	{
		extern AssetPtr<Texture2D> BlankTexture;
		extern AssetPtr<Material> BlankMaterial;
	}
}

#endif //HANDMADE_DEFAULTMATERIALS_H