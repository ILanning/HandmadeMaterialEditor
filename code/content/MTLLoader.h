#ifndef HANDMADE_MATERIAL_LOADER
#define HANDMADE_MATERIAL_LOADER

#include "../handmade_typedefs.h"
#include "../general/StretchyArray.h"
#include "../general/StringHelpers.cpp"
#include "../general/PathHelpers.cpp"
#include "../drawing/Texture2D.h"
#include "../drawing/Material.h"
#include "../math/Vector3.h"
#include "../math/Vector2.h"
#include "MTLTextureOptions.h"
#include <cstdlib>

namespace Content
{
	namespace OBJ
	{
		Vector3 ParseMTLVec3(char *string, int32 length, int32 offset, Vector3 defaultValue = { }, int32 *readFinishIndex = nullptr)
		{
			Vector3 result = defaultValue;
			char *nextFloat = string + offset;

			for (int32 i = 0; i < Vector3::ElementCount; i++)
			{
				char *readEnd = nullptr;
				float value = strtof(nextFloat, &readEnd);
				if (readEnd == nextFloat || (int32)(readEnd - string) > length) //TODO(Ian): Test this with whitespace
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
				if (readEnd == nextFloat || (int32)(readEnd - string) > length) //TODO(Ian): Test this with whitespace
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
			if (!(readEnd == nextFloat || (int32)(readEnd - string) > length)) //TODO(Ian): Test this with whitespace
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

		MTLTextureOptions ParseMapLineOptions(char *string, int32 length, int32 offset, bool isScalar, int32 *readFinishIndex = nullptr)
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

			//TODO(Ian):  This function will cause issues if the texture's filename starts with a number, 
			//            rewind slightly if something other than a space directly follows the last number read?
			int32 currPos = CString::FindNonWhitespace(string, length, offset);
			while (currPos < length)
			{
				if (string[currPos] == '-')
				{
					currPos++;
					if (CString::FindSubstring("blendu", 6, string, 6, currPos))
					{
						currPos += 7;
						options.BlendU = ParseMTLBool(string, length, currPos, true, &currPos);
					}
					else if (CString::FindSubstring("blendv", 6, string, 6, currPos))
					{
						currPos += 7;
						options.BlendV = ParseMTLBool(string, length, currPos, true, &currPos);
					}
					else if (CString::FindSubstring("cc", 2, string, 2, currPos))
					{
						currPos += 3;
						options.ColorCorrection = ParseMTLBool(string, length, currPos, true, &currPos);
					}
					else if (CString::FindSubstring("clamp", 5, string, 5, currPos))
					{
						currPos += 6;
						options.Clamp = ParseMTLBool(string, length, currPos, true, &currPos);
					}
					else if (CString::FindSubstring("mm", 2, string, 2, currPos))
					{
						currPos += 3;
						options.ColorMod = ParseMTLVec2(string, length, currPos, { 1, 0 }, &currPos);
					}
					else if (CString::FindSubstring("texres", 6, string, 6, currPos))
					{
						currPos += 7;
						//options.ColorMod = ParseMTLVec2(string, length, currPos, { 1, 0 }, &currPos);
					}
					else if (CString::FindSubstring("imfchan", 7, string, 7, currPos))
					{
						currPos += 8;

					}
					else if (CString::FindSubstring("bm", 2, string, 2, currPos))
					{
						currPos += 3;
						options.BumpMultiplier = ParseMTLFloat(string, length, currPos, 1, &currPos);
					}
					else if (string[currPos] == 'o')
					{
						currPos += 2;
						options.Offset = ParseMTLVec3(string, length, currPos, { 1, 0 }, &currPos);
					}
					else if (string[currPos] == 's')
					{
						currPos += 2;
						options.Scale = ParseMTLVec3(string, length, currPos, { 1, 0 }, &currPos);
					}
					else if (string[currPos] == 't')
					{
						currPos += 2;
						options.Turbulence = ParseMTLVec3(string, length, currPos, { 1, 0 }, &currPos);
					}
				}
				else
				{
					int32 endPos = CString::FindCharacter(string, ' ', length, currPos);
					options.TexturePath = CString::CopySubstring(string, endPos - currPos, &options.PathLength, length, currPos);
					currPos += endPos;
				}
				currPos = CString::FindNonWhitespace(string, length, currPos);
			}

			if (readFinishIndex)
			{
				*readFinishIndex = currPos;
			}
			return options;
		}

		///Returns a *collection* of materials
		Material *ParseMTL(FileData toLoad, int32 &outMaterialCount)
		{
			bool endOfFile = false;
			bool materialFound = false;
			char *file = (char *)toLoad.File;
			int32 fileLength = toLoad.FileSize;
			StretchyArray<Material> materials = StretchyArray<Material>();
			Material currentMaterial = Material();

			int32 nextLineStart = CString::FindNonWhitespace(file, fileLength, 0);
			int32 nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);

			while (!endOfFile)
			{
				if (nextLineEnd == fileLength)
				{
					endOfFile = true;
				}
				
				if (CString::FindSubstring("newmtl", 6, file, nextLineStart + 6, nextLineStart)) //New material definition
				{
					if (!materialFound)
					{
						materialFound = true;
					}
					materials.PushBack(currentMaterial);

					currentMaterial = Material();
					int32 nameLength = nextLineEnd - (nextLineStart + 7);
					currentMaterial.Name = CString::CopySubstring(file, nameLength, &currentMaterial.NameLength, fileLength, nextLineStart + 7);
				}
				else if (file[nextLineStart] == 'K' || file[nextLineStart] == 'T')
				{
					//TODO(Ian):  Handle xyz and spectral forms of these, if they're ever used in reality

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
				else if (CString::FindSubstring("Ns", 2, file, nextLineStart + 2, nextLineStart)) //Specular exponent/gloss definition
				{
					currentMaterial.Gloss = strtof(file + nextLineStart + 2, nullptr);
				}
				else if (CString::FindSubstring("Ni", 2, file, nextLineStart + 2, nextLineStart))
				{
					currentMaterial.OpticalDensity = strtof(file + nextLineStart + 2, nullptr);
				}
				else if (file[nextLineStart] == 'd') //Dissolve level
				{
					int32 offset = 2;
					if (CString::FindSubstring("-halo", 5, file, nextLineStart + 5, nextLineStart + offset)) //Dissolve less at edges
					{
						currentMaterial.DissolveHalo = true;
						offset += 5;
					}

					currentMaterial.Dissolve = strtof(file + nextLineStart + offset, nullptr);
				}
				else if (CString::FindSubstring("sharpness", 9, file, nextLineStart + 9, nextLineStart)) //Reflection sharpness
				{
					currentMaterial.ReflectionSharpness = strtof(file + nextLineStart + 9, nullptr);
				}
				else if (CString::FindSubstring("illum", 5, file, nextLineStart + 5, nextLineStart)) //Rendering method
				{
					currentMaterial.IlluminationModel = (uint8)strtof(file + nextLineStart + 5, nullptr);
				}
				else if (CString::FindSubstring("map_", 4, file, nextLineStart + 4, nextLineStart)) //Defines a texture map
				{
					int32 offset = nextLineStart + 4;

					if (file[offset] == 'K')
					{
						offset++;
						MTLTextureOptions options = ParseMapLineOptions(file, nextLineEnd, offset + 1, false);
						Texture2D *map = nullptr;

						if (file[offset] == 'a') //Ambient definition
						{
							currentMaterial.AmbientMap = map;
						}
						else if (file[offset] == 'd') //Diffuse definition
						{
							currentMaterial.DiffuseMap = map;
						}
						else if (file[offset] == 's') //Specular definition
						{
							currentMaterial.SpecularMap = map;
						}
					}
					else if (CString::FindSubstring("Ns", 2, file, 2, offset)) //Specular exponent/gloss definition
					{
						offset += 3;
						MTLTextureOptions options = ParseMapLineOptions(file, nextLineEnd, offset + 1, false);
						//currentMaterial.GlossTexture = 
					}
					else if (file[offset] == 'd')
					{
						offset += 2;
						MTLTextureOptions options = ParseMapLineOptions(file, nextLineEnd, offset + 1, false);
						//currentMaterial.DissolveTexture = 
					}
					
					//map_aat on - Turns on antialiasing for textures in this material without affecting all textures, 
					//			   probably not something we care about
				}
				else if (CString::FindSubstring("bump", 4, file, nextLineStart + 4, nextLineStart))
				{

				}
				else if (CString::FindSubstring("decal", 5, file, nextLineStart + 5, nextLineStart))
				{
					//TODO(Ian): Decal support
				}
				else if (CString::FindSubstring("refl", 4, file, nextLineStart + 4, nextLineStart)) //Reflection map definition
				{
					//TODO(Ian): Reflection map support
					//           Needs to handle single texture sphere maps and 6 texture cube maps
				}

				nextLineStart = CString::FindNonWhitespace(file, fileLength, nextLineEnd);
				nextLineEnd = CString::FindLineEnd(file, fileLength, nextLineStart);
			}

			if (materialFound)
			{
				materials.PushBack(currentMaterial);

				outMaterialCount = materials.Length();
				return materials.ToArray();
			}
			else
			{
				//TODO(Ian): Logging, provide a way to report a failure to find any materials
				outMaterialCount = 0;
				return nullptr;
			}
		}

		Material *ParseMTL(char *path, int32 pathLength, ReadFileFunc *readFile, int32 &outMaterialCount)
		{
			bool success = false;
			FileData file = readFile(path, pathLength, &success);
			if (success)
			{
				return ParseMTL(file, outMaterialCount);
			}
			else
			{
				//TODO(Ian): Logging, provide a way to report a failure to load the file
				outMaterialCount = 0;
				return nullptr;
			}
		}
	}
}

#endif