#ifndef HANDMADE_MTLLOADER_CPP
#define HANDMADE_MTLLOADER_CPP

#include "MTLLoader.h"
#include "../general/StaticArray.h"
#include "../general/ArrayList.h"
#include "../general/memory/NewDeleteArena.h"
#include "../general/PathHelpers.h"
#include "../general/StringHelpers.h"
#include "AssetManager.h"
#include "../drawing/Texture2D.h"
#include <cstdlib>

namespace Content
{
	namespace OBJ
	{
		using namespace Drawing;

		namespace _OBJInternal
		{
			Vector3 ParseMTLVec3(char *string, int32 length, int32 offset, Vector3 defaultValue = {}, int32 *readFinishIndex = nullptr)
			{
				Vector3 result = defaultValue;
				char *nextFloat = string + offset;

				for (int32 i = 0; i < Vector3::ElementCount; i++)
				{
					char *readEnd = nullptr;
					float value = strtof(nextFloat, &readEnd);
					if (readEnd == nextFloat || (int32)(readEnd - string) > length) //TODO: Test this with whitespace
					{
						break;
					}
					result.elements[i] = value;
					nextFloat = readEnd;
				}

				if (readFinishIndex)
				{
					*readFinishIndex = (int32)(nextFloat - string);
				}
				return result;
			}

			Vector2 ParseMTLVec2(char *string, int32 length, int32 offset, Vector2 defaultValue, int32 *readFinishIndex = nullptr)
			{
				Vector2 result = defaultValue;
				char *nextFloat = string + offset;

				for (int32 i = 0; i < Vector2::ElementCount; i++)
				{
					char *readEnd = nullptr;
					float value = strtof(nextFloat, &readEnd);
					if (readEnd == nextFloat || (int32)(readEnd - string) > length) //TODO: Test this with whitespace
					{
						break;
					}
					result.elements[i] = value;
					nextFloat = readEnd;
				}

				if (readFinishIndex)
				{
					*readFinishIndex = (int32)(nextFloat - string);
				}
				return result;
			}

			float ParseMTLFloat(char *string, int32 length, int32 offset, float defaultValue, int32 *readFinishIndex = nullptr)
			{
				float result = defaultValue;
				char *nextFloat = string + offset;

				char *readEnd = nullptr;
				float value = strtof(nextFloat, &readEnd);
				if (!(readEnd == nextFloat || (int32)(readEnd - string) > length)) //TODO: Test this with whitespace
				{
					result = value;
					nextFloat = readEnd;
				}

				if (readFinishIndex)
				{
					*readFinishIndex = (int32)(nextFloat - string);
				}
				return result;
			}

			bool ParseMTLBool(char *string, int32 length, int32 offset, bool defaultValue, int32 *readFinishIndex = nullptr)
			{
				bool result = defaultValue;

				if (string[offset + 2] == 'n') //on
				{
					result = true;
					offset += 3;
				}
				else if (string[offset + 2] == 'f') //off
				{
					result = false;
					offset += 4;
				}

				if (readFinishIndex)
				{
					*readFinishIndex = offset;
				}

				return result;
			}

			MTLTextureOptions ParseMapLineOptions(char *line, int32 lineLength, int32 offset, char *folder, bool isScalar, int32 *readFinishIndex = nullptr)
			{
				/*
				Ka, Kd, Ks
				-blendu on | off                   (Texture blending (?) - u direction)
				-blendv on | off                   (Texture blending (?) - v direction)
				-cc on | off                       (Color correction)
				-clamp on | off                    (Texture clamping - restricts u and v to the range 0.0-1.0)
				-mm base gain                      (Modifies the colors in the texture: color = (start * gain) + base)
				(For these three, v and w are *optional*)
				-o u v w                           (Offsets the origin of the texture)
				-s u v w                           (Scales the texture)
				-t u v w                           (Adds variation to the texture procedurally)
				-texres value                      (Specifies the resolution of the final texture)

				Ns, d, bump, disp, decal
				-blendu on | off
				-blendv on | off
				-clamp on | off
				-imfchan r | g | b | m | l | z     (Specifies which channel a texture's data is found in within an image)
				-mm base gain
				-o u v w
				-s u v w
				-t u v w
				-texres value

				bump
				-bm mult                           (Multiply all values in the bump map by this)
				*/
				MTLTextureOptions options = {};

				//TODO:  This function will cause issues if the texture's filename starts with a number, 
				//       rewind slightly if something other than a space directly follows the last number read?
				int32 currPos = CString::FindNonWhitespace(line, lineLength, offset);
				while (currPos != -1)
				{
					if (line[currPos] == '-')
					{
						currPos++;
						if (CString::FindSubstring("blendu", 6, line + currPos, 6) != -1)
						{
							currPos += 7;
							options.BlendU = ParseMTLBool(line, lineLength, currPos, true, &currPos);
						}
						else if (CString::FindSubstring("blendv", 6, line + currPos, 6) != -1)
						{
							currPos += 7;
							options.BlendV = ParseMTLBool(line, lineLength, currPos, true, &currPos);
						}
						else if (CString::FindSubstring("cc", 2, line + currPos, 2) != -1)
						{
							currPos += 3;
							options.ColorCorrection = ParseMTLBool(line, lineLength, currPos, true, &currPos);
						}
						else if (CString::FindSubstring("clamp", 5, line + currPos, 5) != -1)
						{
							currPos += 6;
							options.Clamp = ParseMTLBool(line, lineLength, currPos, true, &currPos);
						}
						else if (CString::FindSubstring("mm", 2, line + currPos, 2) != -1)
						{
							currPos += 3;
							options.ColorMod = ParseMTLVec2(line, lineLength, currPos, { 1, 0 }, &currPos);
						}
						else if (CString::FindSubstring("texres", 6, line + currPos, 6) != -1)
						{
							currPos += 7;
							//options.ColorMod = ParseMTLVec2(string, length, currPos, { 1, 0 }, &currPos);
						}
						else if (CString::FindSubstring("imfchan", 7, line + currPos, 7) != -1)
						{
							currPos += 8;

						}
						else if (CString::FindSubstring("bm", 2, line + currPos, 2) != -1)
						{
							currPos += 3;
							options.BumpMultiplier = ParseMTLFloat(line, lineLength, currPos, 1, &currPos);
						}
						else if (line[currPos] == 'o')
						{
							currPos += 2;
							options.Offset = ParseMTLVec3(line, lineLength, currPos, { 1, 0 }, &currPos);
						}
						else if (line[currPos] == 's')
						{
							currPos += 2;
							options.Scale = ParseMTLVec3(line, lineLength, currPos, { 1, 0 }, &currPos);
						}
						else if (line[currPos] == 't')
						{
							currPos += 2;
							options.Turbulence = ParseMTLVec3(line, lineLength, currPos, { 1, 0 }, &currPos);
						}
					}
					else
					{
						int32 endPos = CString::FindCharacter(line, ' ', lineLength, currPos);
						if (endPos == -1)
						{
							endPos = lineLength;
						}
						int32 pathLength = 0;
						char *path = CString::CopySubstring(line + currPos, endPos - currPos, &pathLength, lineLength);
						if (Path::IsRelative(path))
						{
							char *pathEnd = path;

							path = Path::Combine(folder, pathEnd, 0, 0, &pathLength);

							delete[] pathEnd;
						}

						options.Path = { path, (uint32)pathLength };
						currPos += endPos;
					}
					currPos = CString::FindNonWhitespace(line, lineLength, currPos);
				}

				if (readFinishIndex)
				{
					*readFinishIndex = currPos;
				}
				return options;
			}
		
			void FinalizeMaterial(const FileData& toLoad, const HMString& matName, Material& mat, 
				Collections::ArrayList<NamedMTL, Memory::NewDeleteArena>& materialList, AssetManager& assets)
			{
				//Create a name for the new material, then add it to the AssetManager
				int32 assetNameLength = matName.Length() + toLoad.PathSize;
				char* assetName = new char[assetNameLength];
				CString::WriteChars(assetName, assetNameLength, toLoad.Path, toLoad.PathSize - 1);
				assetName[toLoad.PathSize - 1] = ' ';
				CString::WriteChars(assetName + toLoad.PathSize, matName.Length(), matName.RawCString(), matName.Length());

				bool success = false;
				AssetPtr<Material> finalMat = assets.AddManaged(mat, { assetName, (uint32)assetNameLength }, success);

				materialList.Add({ matName, finalMat });
			}
		}

		void ParseMTL(FileData toLoad, Collections::ArrayList<NamedMTL, Memory::NewDeleteArena>& materialList, AssetManager& assets)
		{
			using namespace _OBJInternal;

			bool endOfFile = false;
			bool materialFound = false;
			char *file = (char *)toLoad.File;
			char *fileDirectory = Path::CloneParentDirectory(toLoad.Path);
			int32 fileLength = toLoad.FileSize;
			Material currentMaterial = Material();
			HMString currentMatName = {};

			int32 nextLineStart = CString::FindNonWhitespace(file, fileLength);
			int32 nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);

			while (!endOfFile)
			{
				if (nextLineEnd == fileLength)
				{
					endOfFile = true;
				}

				if (CString::FindSubstring("newmtl", 6, file + nextLineStart, 6) != -1) //New material definition
				{
					if (!materialFound)
					{
						materialFound = true;
					}
					else
					{
						FinalizeMaterial(toLoad, currentMatName, currentMaterial, materialList, assets);
					}

					currentMaterial = Material();
					int32 nameLength = nextLineEnd - (nextLineStart + 7);
					char* name = CString::CopySubstring(file + nextLineStart + 7, nameLength, &nameLength, fileLength);
					currentMatName = { name, (uint32)nameLength };
				}
				else if (file[nextLineStart] == 'K' || file[nextLineStart] == 'T')
				{
					//TODO:  Handle xyz and spectral forms of these, if they're ever used in reality

					Vector3 color = ParseMTLVec3(file, fileLength, nextLineStart + 2, { 1, 1, 1 });

					if (file[nextLineStart + 1] == 'a') //Ambient definition
					{
						currentMaterial.AmbientColor = color;
					}
					else if (file[nextLineStart + 1] == 'd') //Diffuse definition
					{
						currentMaterial.DiffuseColor = color;
					}
					else if (file[nextLineStart + 1] == 's') //Specular definition
					{
						currentMaterial.SpecularColor = color;
					}
					else if (file[nextLineStart + 1] == 'f') //Transmission filter definition
					{
						currentMaterial.TransmissionFilter = color;
					}
				}
				else if (CString::FindSubstring("Ns", 2, file + nextLineStart, 2) != -1) //Specular exponent/gloss definition
				{
					currentMaterial.Gloss = strtof(file + nextLineStart + 2, nullptr);
				}
				else if (CString::FindSubstring("Ni", 2, file + nextLineStart, nextLineStart + 2) != -1)
				{
					currentMaterial.OpticalDensity = strtof(file + nextLineStart + 2, nullptr);
				}
				else if (file[nextLineStart] == 'd') //Dissolve level
				{
					int32 offset = 2;
					if (CString::FindSubstring("-halo", 5, file + nextLineStart, 5) != -1) //Dissolve less at edges
					{
						currentMaterial.DissolveHalo = true;
						offset += 5;
					}

					currentMaterial.Dissolve = strtof(file + nextLineStart + offset, nullptr);
				}
				else if (CString::FindSubstring("sharpness", 9, file + nextLineStart, 9) != -1) //Reflection sharpness
				{
					currentMaterial.ReflectionSharpness = strtof(file + nextLineStart + 9, nullptr);
				}
				else if (CString::FindSubstring("illum", 5, file + nextLineStart, 5) != -1) //Rendering method
				{
					currentMaterial.IlluminationModel = (uint8)strtof(file + nextLineStart + 5, nullptr);
				}
				else if (CString::FindSubstring("map_", 4, file + nextLineStart, 4) != -1) //Defines a texture map
				{
					int32 offset = nextLineStart + 4;
					bool success = false;

					if (file[offset] == 'K')
					{
						offset++;
						MTLTextureOptions options = ParseMapLineOptions(file, nextLineEnd, offset + 1, fileDirectory, false);

						if (file[offset] == 'a') //Ambient definition
						{
							AssetPtr<Texture2D> map = assets.Load(options, Content::TextureMapType::Ambient, success);
							if (success)
							{
								currentMaterial.AmbientMap = map;
							}
						}
						else if (file[offset] == 'd') //Diffuse definition
						{
							AssetPtr<Texture2D> map = assets.Load(options, Content::TextureMapType::Diffuse, success);
							if (success)
							{
								currentMaterial.DiffuseMap = map;
							}
						}
						else if (file[offset] == 's') //Specular definition
						{
							AssetPtr<Texture2D> map = assets.Load(options, Content::TextureMapType::Specular, success);
							if (success)
							{
								currentMaterial.SpecularMap = map;
							}
						}
					}
					else if (CString::FindSubstring("Ns", 2, file + offset, 2) != -1) //Specular exponent/gloss definition
					{
						offset += 3;
						MTLTextureOptions options = ParseMapLineOptions(file, nextLineEnd, offset + 1, fileDirectory, false);
						AssetPtr<Texture2D> map = assets.Load(options, Content::TextureMapType::Gloss, success);
						if (success)
						{
							currentMaterial.GlossMap = map;
						}
					}
					else if (file[offset] == 'd')
					{
						offset += 2;
						MTLTextureOptions options = ParseMapLineOptions(file, nextLineEnd, offset + 1, fileDirectory, false);
						AssetPtr<Texture2D> map = assets.Load(options, Content::TextureMapType::Dissolve, success);
						if (success) 
						{
							currentMaterial.DissolveMap = map;
						}
					}

					//map_aat on - Turns on antialiasing for textures in this material without affecting all textures, 
					//			   probably not something we care about
				}
				else if (CString::FindSubstring("bump", 4, file + nextLineStart, 4) != -1)
				{

				}
				else if (CString::FindSubstring("decal", 5, file + nextLineStart, 5) != -1)
				{
					//TODO: Decal support
				}
				else if (CString::FindSubstring("refl", 4, file + nextLineStart, 4) != -1) //Reflection map definition
				{
					//TODO: Reflection map support
					//      Needs to handle single texture sphere maps and 6 texture cube maps
				}

				nextLineStart = CString::FindNonWhitespace(file, fileLength, nextLineEnd);
				if (nextLineStart == -1)
				{
					break;
				}
				nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);

			}

			delete[] fileDirectory;

			if (materialFound)
			{
				FinalizeMaterial(toLoad, currentMatName, currentMaterial, materialList, assets);
			}
			else
			{
				//TODO: Logging, provide a way to report a failure to find any materials
				return;
			}
		}

		StaticArray<NamedMTL> ParseMTL(FileData file, Memory::NewDeleteArena& arena, AssetManager &assets)
		{
			auto mats = Collections::ArrayList<NamedMTL, Memory::NewDeleteArena>(&arena);
			ParseMTL(file, mats, assets);

			if (mats.Length() > 0)
			{
				auto *outputArray = mats.ToArray(arena);
				return { outputArray, (uint32)mats.Length() };
			}
			else
			{
				return { nullptr, 0 };
			}
		}

		void ParseMTL(HMString path, ReadFileFunc* readFile, Collections::ArrayList<NamedMTL, Memory::NewDeleteArena>& materialList, AssetManager &assets)
		{
			bool success = false;
			FileData file = readFile(path.RawCString(), path.Length(), &success);
			if (success)
			{
				ParseMTL(file, materialList, assets);
			}
			else
			{
				//TODO: Logging, provide a way to report a failure to load the file
				return;
			}
		}

		StaticArray<NamedMTL> ParseMTL(HMString path, ReadFileFunc *readFile, Memory::NewDeleteArena& arena, AssetManager &assets)
		{
			bool success = false;
			FileData file = readFile(path.RawCString(), path.Length(), &success);
			if (success)
			{
				return ParseMTL(file, arena, assets);
			}
			else
			{
				//TODO: Logging, provide a way to report a failure to load the file
				return{ nullptr, 0 };
			}
		}
	}
}

#endif // !HANDMADE_MTLLOADER_CPP
