#ifndef HANDMADE_CONTENT_SHADERMANAGER_H
#define HANDMADE_CONTENT_SHADERMANAGER_H

#include "ShaderPart.h"
#include "ShaderProgram.h"
#include "../general/ArrayList.h"
#include "../general/HashMap.h"
#include "../general/memory/NewDeleteArena.h"
#include "../libraries/glew.h"

namespace Content
{
	///Contains and manages shader programs and shader parts.
	class ShaderManager
	{
	public:
		Collections::ArrayList<ShaderProgram, Memory::NewDeleteArena> ShaderPrograms;
		Collections::ArrayList<ShaderPart, Memory::NewDeleteArena> VertShaders;
		Collections::ArrayList<ShaderPart, Memory::NewDeleteArena> FragShaders;
		Collections::HashMap<HMString, GLuint, Memory::NewDeleteArena> NamedShaderPrograms;

		GLuint DefaultShader = 0;

		ShaderManager(Memory::NewDeleteArena* memory);

		///Create a full new shader program from the given files.
		GLuint CreateShader(const HMString& vertSourcePath, const HMString& fragSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError);

		///Creates a full new shader program from the given files and gives it a name.
		GLuint CreateNamedShader(const HMString& name, const HMString& vertSourcePath, const HMString& fragSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError);
	};
}

#endif //HANDMADE_CONTENT_SHADERMANAGER_H