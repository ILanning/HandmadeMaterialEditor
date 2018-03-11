#ifndef HANDMADE_CONTENTMANAGER_H
#define HANDMADE_CONTENTMANAGER_H

#include "../handmade_typedefs.h"
#include "../general/StretchyArray.h"
#include "../general/StringHelpers.h"
#include "../drawing/Texture2D.h"
#include "OBJLoader.h"

//TODO(Ian): This class is just a dummy for interface design purposes, it still needs to be built!
class ContentManager
{
	StretchyArray<void*> items;
	ReadFileFunc *reader;

public:
	ContentManager(ReadFileFunc *readFile, char *basePath)
	{
		items = StretchyArray<void*>();
		reader = readFile;
	}

	template<class T> T* Load(char *path)
	{
		return nullptr;
	}

	template<class T> T* Load(char *path, int32 pathLength, int32 offset)
	{
		char *finalPath = CopySubstring(path, pathLength, offset);
		T *result = Load<T>(finalPath);
		delete[] finalPath;
		return result;
	}

	template<> Texture2D* Load(char *path)
	{
		int32 x, y, components = 0;
		uint8 *data = stbi_load(path, &x, &y, &components, 4);
		Texture2D *tex = new Texture2D(data, x, y, GL_RGBA, GL_RGBA);
		items.PushBack(tex);

		return tex;
	}

	template<> Geometry* Load(char *path)
	{
		Content::OBJ::ObjParser *parser = new Content::OBJ::ObjParser(path, FindSubstring("", 1, path, MaxInt32), reader);

		Assert(false); //Not implemented error!

		return nullptr;
	}

	void Register(void *object, char *name)
	{
		items.PushBack(object);
	}

	~ContentManager()
	{
		int32 count = items.Length();
		for (int32 i = 0; i < count; i++)
		{
			delete items[i];
		}
	}
};

#endif //HANDMADE_CONTENTMANAGER_H