#ifndef HANDMADE_DEFAULTMATERIALS
#define HANDMADE_DEFAULTMATERIALS

#include "../../handmade_typedefs.h"
#include "../general/StringHelpers.cpp"
#include "Texture2D.h"
#include "Material.h"

//TODO(Ian): Probably package these up into something rather than leaving them hanging in the global namespace
//           Maybe part of a package of named defaults associated with the ContentManager?
//           ie. pass Load<Texture2D>(string) "White1x1" to get this, no file load needed
namespace Drawing
{
	namespace Defaults
	{
		Texture2D *BlankTexture = new Texture2D(new uint8[4]{ 255, 255, 255, 255 }, 1, 1, GL_RGBA, GL_RGBA, GL_REPEAT, CString::CopySubstring("White1x1", 8), 9);
		Material *BlankMaterial = new Material("BlankFlat", 10, BlankTexture);
	}
}

#endif