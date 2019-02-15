#ifndef HANDMADE_CONTENT_SHADERMANAGER_H
#define HANDMADE_CONTENT_SHADERMANAGER_H

#include "ShaderPart.h"
#include "ShaderProgram.h"
#include "../handmade_typedefs.h"
#include "../general/ArrayList.h"
#include "../general/HashMap.h"
#include "../general/memory/NewDeleteArena.h"

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

		ShaderManager(Memory::NewDeleteArena* memory) 
		{
			ShaderPrograms = Collections::ArrayList<ShaderProgram, Memory::NewDeleteArena>(memory, 4);
			VertShaders = Collections::ArrayList<ShaderPart, Memory::NewDeleteArena>(memory, 4);
			FragShaders = Collections::ArrayList<ShaderPart, Memory::NewDeleteArena>(memory, 4);
			NamedShaderPrograms = Collections::HashMap<HMString, GLuint, Memory::NewDeleteArena>(4, memory);
		}

		///Create a full new shader program from the given files.
		GLuint CreateShader(HMString& vertSourcePath, HMString& fragSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError)
		{
			ShaderPart vert = ShaderPart(vertSourcePath, readFile, GL_VERTEX_SHADER);
			VertShaders.EnsureCapacity(vert.ID);
			VertShaders[vert.ID] = vert;

			ShaderPart frag = ShaderPart(fragSourcePath, readFile, GL_FRAGMENT_SHADER);
			FragShaders.EnsureCapacity(frag.ID);
			FragShaders[frag.ID] = frag;

			ShaderProgram program = ShaderProgram(vert.ID, frag.ID, messageError);
			ShaderPrograms.EnsureCapacity(program.ID);
			ShaderPrograms[program.ID] = program;

			return program.ID;
		}

		///Creates a full new shader program from the given files and gives it a name.
		GLuint CreateNamedShader(HMString& name, HMString& vertSourcePath, HMString& fragSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError)
		{
			GLuint shaderID = CreateShader(vertSourcePath, fragSourcePath, readFile, messageError);
			NamedShaderPrograms[name] = shaderID;

			return shaderID;
		}
	};
}

#endif //HANDMADE_CONTENT_SHADERMANAGER_H