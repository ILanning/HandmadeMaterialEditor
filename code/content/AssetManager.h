#ifndef HANDMADE_ASSETMANAGER_H
#define HANDMADE_ASSETMANAGER_H

#include <new>
#include "../handmade_typedefs.h"
#include "../content/MTLLoader.h"
#include "../drawing/Geometry.h"
#include "../drawing/Material.h"
#include "../drawing/Model.h"
#include "../drawing/Sprite.h"
#include "../drawing/Texture2D.h"
#include "../general/HashMap.h"
#include "../general/HMString.h"
#include "../general/StaticArray.h"
#include "../general/StringHelpers.cpp"
#include "../general/memory/NewDeleteArena.h"
#include "OBJLoader.h"

//A clas for managing large assets such as textures and model geometry.
class AssetManager
{
	struct Asset
	{
		bool Managed = false;
		void* Data = nullptr;
	};

	Collections::HashMap<HMString, Asset, Memory::NewDeleteArena> items = {};
	ReadFileFunc* reader = nullptr;
	HMString basePath = {};
	Memory::NewDeleteArena memory;

	typedef Collections::HashMap<HMString, Drawing::Material, Memory::NewDeleteArena> MTLDict;

public:
	AssetManager() {}

	AssetManager(ReadFileFunc* readFile, Memory::NewDeleteArena& arena, HMString basePath = {}) : reader(readFile), basePath(basePath),
		items(Collections::HashMap<HMString, Asset, Memory::NewDeleteArena>(&arena)), memory(arena)
	{}

	template<typename T = void>
	//Loads an item into the AssetManager and returns a reference to it, or just returns a reference if it's already loaded in.  
	//Will not attempt to load asset classes it does not understand from file, but will return them if they've been Register()'d.
	T* Load(HMString path)
	{
		if (items.CheckExists(path))
		{
			return (T*)items[path].Data;
		}
		return nullptr;
	}

#ifdef HANDMADE_INTERNAL
	//Note:  Temporary hack until a proper shader handling system exists, remove ASAP
	GLuint shaderProgram;
#endif

	template<>
	Drawing::Texture2D* Load(HMString path)
	{
		if (!items.CheckExists(path))
		{
			Drawing::Texture2D* tex = memory.Allocate<Drawing::Texture2D>();
			new (tex) Drawing::Texture2D(path.RawCString());

			items[path] = { true, tex };
			return tex;
		}
		else
		{
			return (Drawing::Texture2D*)items[path].Data;
		}
	}

	template<>
	MTLDict* Load(HMString path)
	{
		if (!items.CheckExists(path))
		{
			MTLDict *mats = memory.Allocate<MTLDict>();
			new (mats) MTLDict(Content::OBJ::ParseMTL(path, reader, memory), &memory);

			items[path] = { true, mats };
			return mats;
		}
		else
		{
			return (MTLDict*)items[path].Data;
		}
	}

	template<>
	Drawing::Geometry* Load(HMString path)
	{
		if (!items.CheckExists(path))
		{
			Drawing::Texture2D *tex = memory.Allocate<Drawing::Texture2D>();
			tex = new (tex) Drawing::Texture2D(path.RawCString());
			Drawing::Geometry* geo = Content::ParseOBJ(path.RawCString(), path.Length(), shaderProgram, reader);

			items[path] = { true, geo };
			return geo;
		}
		else
		{
			return (Drawing::Geometry*)items[path].Data;
		}
	}

	//Registers a pointer to an object with the AssetManager without doing any processing to the object.  Object may be of any type, not just those that are loadable.
	template<typename T>
	T Register(T* object, HMString name)
	{
		items[name] = { false, object };
	}

	//Removes an item from the AssetManager.
	void Remove(HMString name)
	{
		Asset item = items[name];
		if (item.Managed)
		{
			memory.Deallocate(item.Data);
		}
		items.Remove(name);
	}
};

#endif //HANDMADE_ASSETMANAGER_H