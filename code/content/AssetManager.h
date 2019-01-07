#ifndef HANDMADE_ASSETMANAGER_H
#define HANDMADE_ASSETMANAGER_H

#include <new>
#include "../handmade_typedefs.h"
#include "AssetPtr.h"
#include "MTLDict.h"
#include "MTLTextureOptions.h"
#include "MeshCollection.h"
#include "ShaderManager.h"
#include "../drawing/Material.h"
#include "../drawing/Model.h"
#include "../drawing/Texture2D.h"
#include "../general/HashMap.h"
#include "../general/HMString.h"
#include "../general/memory/NewDeleteArena.h"

//A class for managing large assets such as textures and model geometry.
class AssetManager
{
	struct Asset
	{
		bool Managed = false;
		Content::AssetPtrSharedData* AssetTracker = {};
		void* Data = nullptr;

		Asset() {}

		Asset(bool managed, void* data) : Managed(managed), Data(data) {}
	};

	Collections::HashMap<HMString, Asset, Memory::NewDeleteArena> items = {};
	ReadFileFunc* reader = nullptr;
	HMString basePath = {};
	Memory::NewDeleteArena memory;
	Content::AssetPtrSharedData invalidAssetData = {1};

public:
	Content::ShaderManager Shaders;

	AssetManager(ReadFileFunc* readFile, Memory::NewDeleteArena& arena, HMString basePath = {}) : reader(readFile), basePath(basePath),
		items(Collections::HashMap<HMString, Asset, Memory::NewDeleteArena>(&arena)), memory(arena), Shaders(Content::ShaderManager(&arena))
	{
	}

	template<typename T = void>
	//Loads an item into the AssetManager and returns a reference to it, or just returns a reference if it's already loaded in.  
	//Will not attempt to load asset classes it does not understand from file, but will return them if they've been Register()'d.
	AssetPtr<T> Load(HMString path, bool& validAsset);

	template<>
	AssetPtr<Drawing::Texture2D> Load(HMString path, bool& validAsset);

	template<>
	AssetPtr<Content::MTLDict> Load(HMString path, bool& validAsset);

	template<>
	AssetPtr<Content::MeshCollection> Load(HMString path, bool& validAsset);

	AssetPtr<Drawing::Texture2D> Load(Content::OBJ::MTLTextureOptions& options, Content::TextureMapType mapType, bool& validAsset);

	GLuint CreateShader(HMString name, HMString vertSourcePath, HMString fragSourcePath, DebugMessageErrorFunc *messageError);

	//Returns the ID of the requested prebuilt ShaderProgram.
	GLuint GetShader(HMString name, bool& success);

	/**Creates a copy of the object for the AssetManager without doing any other processing to the object.
	Object may be of any type, not just those that are loadable.  Fails if the given name already exists in the AssetManager.
	*/
	template<typename T>
	AssetPtr<T> AddManaged(T &object, HMString name, bool& success);

	/**Registers a pointer to an object with the AssetManager without doing any processing to the object.
	Object may be of any type, not just those that are loadable.  Fails if the given name already exists in the AssetManager.
	*/
	template<typename T>
	AssetPtr<T> AddUnmanaged(T* object, HMString name, bool& success);

	//TODO: Unload(HMString name), which frees the memory used by an asset but does not remove it from the dictionary (and therefore leave dangling AssetPtrs)

	//Removes an item from the AssetManager.
	void Remove(HMString name);
};

#endif //HANDMADE_ASSETMANAGER_H