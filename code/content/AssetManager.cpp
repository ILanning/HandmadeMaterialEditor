#ifndef HANDMADE_ASSETMANAGER_CPP
#define HANDMADE_ASSETMANAGER_CPP

#include "AssetManager.h"
#include "MTLLoader.h"
#include "OBJLoader.cpp"
#include "../general/StaticArray.h"
#include "../general/StringHelpers.cpp"

template<typename T>
AssetPtr<T> AssetManager::Load(HMString path, bool& validAsset)
{
	if (items.CheckExists(path))
	{
		validAsset = true;
		return AssetPtr<T>((T*)items[path].Data, &items[path].AssetTracker);
	}
	validAsset = false;
	return AssetPtr<T>(&invalidAssetData, nullptr);
}

template<>
AssetPtr<Drawing::Texture2D> AssetManager::Load(HMString path, bool& validAsset)
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

AssetPtr<Drawing::Texture2D> AssetManager::Load(Content::OBJ::MTLTextureOptions& options, Content::TextureMapType mapType, bool& validAsset)
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

template<>
AssetPtr<Content::MTLDict> AssetManager::Load(HMString path, bool& validAsset)
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

template<>
AssetPtr<Content::MeshCollection> AssetManager::Load(HMString path, bool& validAsset)
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

GLuint AssetManager::CreateShader(HMString name, HMString vertSourcePath, HMString fragSourcePath, DebugMessageErrorFunc *messageError)
{
	return Shaders.SetNamedShader(name, vertSourcePath, fragSourcePath, reader, messageError);
}

GLuint AssetManager::GetShader(HMString name, bool& success)
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

template<typename T>
AssetPtr<T> AssetManager::AddManaged(T &object, HMString name, bool& success)
{
	if (items.CheckExists(name))
	{
		success = false;
		return AssetPtr<T>(nullptr, &invalidAssetData);
	}
	T* item = memory.Allocate<T>();
	memset(item, 0, sizeof(T));
	*item = object;
	items[name] = { true, item };
	items[name].AssetTracker = new (memory.Allocate<Content::AssetPtrSharedData>()) Content::AssetPtrSharedData();
	success = true;
	return AssetPtr<T>(item, items[name].AssetTracker);
}

template<typename T>
AssetPtr<T> AssetManager::AddUnmanaged(T* object, HMString name, bool& success)
{
	if (items.CheckExists(name))
	{
		success = false;
		return AssetPtr<T>(&invalidAssetData, nullptr);
	}
	items[name] = { false, object };
	items[name].AssetTracker = new (memory.Allocate<Content::AssetPtrSharedData>()) Content::AssetPtrSharedData();
	succcess = true;
	return AssetPtr<T>(object, items[name].AssetTracker);
}

void AssetManager::Remove(HMString name)
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