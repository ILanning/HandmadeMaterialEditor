#ifndef HANDMADE_ASSETMANAGER_CPP
#define HANDMADE_ASSETMANAGER_CPP

#include "AssetManager.h"
#include "AssetPtr.h"
#include "MTLLoader.h"
#include "OBJLoader.h"
#include "../handmade_debugfuncs.h"
#include "../drawing/Texture2D.h"
#include "../general/HMString.h"
#include "../general/StaticArray.h"
#include "../general/StringHelpers.h"
#include "../general/memory/NewDeleteArena.h"

AssetPtr<Drawing::Texture2D> AssetManager::internalLoadTexture2D(const HMString path, bool& validAsset)
{
	if (!items.CheckExists(path))
	{
		Drawing::Texture2D* tex = memory.Allocate<Drawing::Texture2D>();
		new (tex) Drawing::Texture2D(path.RawCString());

		items[path] = { true, tex };
		items[path].AssetTracker = new (memory.Allocate<Content::AssetPtrSharedData>()) Content::AssetPtrSharedData();
		validAsset = true;
		return AssetPtr<Drawing::Texture2D>((Drawing::Texture2D*)items[path].Data, items[path].AssetTracker);
	}
	else
	{
		validAsset = true;
		return AssetPtr<Drawing::Texture2D>((Drawing::Texture2D*)items[path].Data, items[path].AssetTracker);
	}
}

AssetPtr<Content::MTLDict> AssetManager::internalLoadMTLDict(const HMString path, bool& validAsset)
{
	if (!items.CheckExists(path))
	{
		Content::MTLDict *mats = memory.Allocate<Content::MTLDict>();
		new (mats) Content::MTLDict(Content::OBJ::ParseMTL(path, reader, memory, *this), &memory);

		items[path] = { true, mats };
		items[path].AssetTracker = new (memory.Allocate<Content::AssetPtrSharedData>()) Content::AssetPtrSharedData();
		validAsset = true;
		return AssetPtr<Content::MTLDict>((Content::MTLDict*)items[path].Data, items[path].AssetTracker);
	}
	else
	{
		validAsset = true;
		return AssetPtr<Content::MTLDict>((Content::MTLDict*)items[path].Data, items[path].AssetTracker);
	}
}

AssetPtr<Content::MeshCollection> AssetManager::internalLoadMeshCollection(const HMString path, bool& validAsset)
{
	if (!items.CheckExists(path))
	{
		Content::MeshCollection *geo = memory.Allocate<Content::MeshCollection>();
		memset(geo, 0, sizeof(Content::MeshCollection));
		//TODO: Have it load in a shader based on the proerties needed by the material
		*geo = Content::ParseOBJ(path.RawCString(), path.Length(), Shaders.DefaultShader, reader, *this, &memory);

		items[path] = { true, geo };
		items[path].AssetTracker = new (memory.Allocate<Content::AssetPtrSharedData>()) Content::AssetPtrSharedData();
		validAsset = true;
		return AssetPtr<Content::MeshCollection>((Content::MeshCollection*)items[path].Data, items[path].AssetTracker);
	}
	else
	{
		validAsset = true;
		return AssetPtr<Content::MeshCollection>((Content::MeshCollection*)items[path].Data, items[path].AssetTracker);
	}
}

AssetPtr<Drawing::Texture2D> AssetManager::Load(const Content::OBJ::MTLTextureOptions& options, const Content::TextureMapType mapType, bool& validAsset)
{
	if (!items.CheckExists(options.Path))
	{
		Drawing::Texture2D* tex = memory.Allocate<Drawing::Texture2D>();
		new (tex) Drawing::Texture2D(options, mapType);

		items[options.Path] = { true, tex };
		items[options.Path].AssetTracker = new (memory.Allocate<Content::AssetPtrSharedData>()) Content::AssetPtrSharedData();
		validAsset = true;
		return AssetPtr<Drawing::Texture2D>((Drawing::Texture2D*)items[options.Path].Data, items[options.Path].AssetTracker);
	}
	else
	{
		validAsset = true;
		return AssetPtr<Drawing::Texture2D>((Drawing::Texture2D*)items[options.Path].Data, items[options.Path].AssetTracker);
	}
}

GLuint AssetManager::CreateShader(const HMString name, const HMString vertSourcePath, const HMString fragSourcePath, DebugMessageErrorFunc *messageError)
{
	return Shaders.CreateNamedShader(name, vertSourcePath, fragSourcePath, reader, messageError);
}

GLuint AssetManager::GetShader(const HMString name, bool& success)
{
	if (Shaders.NamedShaderPrograms.CheckExists(name))
	{
		success = true;
		return Shaders.NamedShaderPrograms[name];
	}
	else
	{
		success = false;
		return 0;
	}
}

void AssetManager::Remove(const HMString name)
{
	Asset *item = items.GetAt(name);
	if (item)
	{
		if (item->Managed)
		{
			memory.Deallocate(item->Data);
		}
		items.Remove(name);
	}
}

#endif //HANDMADE_ASSETMANAGER_CPP