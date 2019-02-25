#ifndef HANDMADE_CONTENT_SHADERMANAGER_CPP
#define HANDMADE_CONTENT_SHADERMANAGER_CPP

#include "ShaderPart.h"
#include "ShaderProgram.h"
#include "ShaderManager.h"
#include "../handmade_typedefs.h"
#include "../handmade_debugfuncs.h"
#include "../general/ArrayList.h"
#include "../general/HashMap.h"
#include "../general/memory/NewDeleteArena.h"

namespace Content
{
		ShaderManager::ShaderManager(Memory::NewDeleteArena* memory)
		{
			ShaderPrograms = Collections::ArrayList<ShaderProgram, Memory::NewDeleteArena>(memory, 4);
			VertShaders = Collections::ArrayList<ShaderPart, Memory::NewDeleteArena>(memory, 4);
			FragShaders = Collections::ArrayList<ShaderPart, Memory::NewDeleteArena>(memory, 4);
			NamedShaderPrograms = Collections::HashMap<HMString, GLuint, Memory::NewDeleteArena>(4, memory);
		}

		///Create a full new shader program from the given files.
		GLuint ShaderManager::CreateShader(const HMString& vertSourcePath, const HMString& fragSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError)
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
		GLuint ShaderManager::CreateNamedShader(const HMString& name, const HMString& vertSourcePath, const HMString& fragSourcePath, ReadFileFunc *readFile, DebugMessageErrorFunc *messageError)
		{
			GLuint shaderID = CreateShader(vertSourcePath, fragSourcePath, readFile, messageError);
			NamedShaderPrograms[name] = shaderID;

			return shaderID;
		}
}

#endif //HANDMADE_CONTENT_SHADERMANAGER_CPP