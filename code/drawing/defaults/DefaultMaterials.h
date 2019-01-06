#ifndef HANDMADE_DEFAULTMATERIALS
#define HANDMADE_DEFAULTMATERIALS

#include "../../handmade_typedefs.h"
#include "../../content/AssetPtr.h"
#include "../../content/AssetPtrSharedData.h"
#include "../../general/StringHelpers.cpp"
#include "../Texture2D.h"
#include "../Material.h"

//TODO: Probably package these up into something rather than leaving them hanging in the global namespace
//      Maybe part of a package of named defaults associated with the ContentManager?
//      ie. pass Load<Texture2D>(string) "White1x1" to get this, no file load needed
namespace Drawing
{
	namespace Defaults
	{
		AssetPtr<Texture2D> BlankTexture = { new Texture2D(new uint8[4]{ 255, 255, 255, 255 }, 1, 1, GL_RGBA, GL_RGBA, GL_REPEAT), new Content::AssetPtrSharedData() };
		AssetPtr<Material> BlankMaterial = { new Material(BlankTexture), new Content::AssetPtrSharedData() };
	}
}

#endif