#ifndef HANDMADE_MATERIAL_H
#define HANDMADE_MATERIAL_H

#include "../handmade_typedefs.h"
#include "../math/Vector3.cpp"
#include "../general/StringHelpers.cpp"
#include "Texture2D.h"

namespace Drawing
{
	struct Material
	{
		char *Name = nullptr;
		int32 NameLength = 0;
		Vector3 AmbientColor = { 1, 1, 1 };
		Texture2D *AmbientMap = nullptr;
		Vector3 DiffuseColor = { 1, 1, 1 };
		Texture2D *DiffuseMap = nullptr;
		Vector3 SpecularColor = { 1, 1, 1 };
		Texture2D *SpecularMap = nullptr;
		real32 Gloss = 1;
		Texture2D *GlossMap = nullptr;
		real32 Dissolve = 1;
		Texture2D *DissolveMap = nullptr;
		Vector3 TransmissionFilter = { 1, 1, 1 };
		real32 ReflectionSharpness = 60;
		real32 OpticalDensity = 1;
		Texture2D *BumpMap = nullptr;
		Texture2D *NormalMap = nullptr;
		Texture2D *ReflectionMap = nullptr;
		bool DissolveHalo = false;
		uint8 IlluminationModel = 0; //Probably replace this with a reference to a shader

		/*
		Illumination    Properties that are turned on in the
		model           Property Editor

		0		Color on and Ambient off
		1		Color on and Ambient on
		2		Highlight on
		3		Reflection on and Ray trace on
		4		Transparency: Glass on
		Reflection: Ray trace on
		5		Reflection: Fresnel on and Ray trace on
		6		Transparency: Refraction on
		Reflection: Fresnel off and Ray trace on
		7		Transparency: Refraction on
		Reflection: Fresnel on and Ray trace on
		8		Reflection on and Ray trace off
		9		Transparency: Glass on
		Reflection: Ray trace off
		10		Casts shadows onto invisible surfaces
		*/

		friend void swap(Material &first, Material &second)
		{
			//ASK(Ian): Any reason in this case to not just do this?
			//Only disadvantage seems to be somewhat more memory used on the stack
			char temp[sizeof(Material)];
			memcpy(temp, &first, sizeof(Material));
			memcpy(&first, &second, sizeof(Material));
			memcpy(&second, temp, sizeof(Material));

			/* *tempString = first.Name;
			first.Name = second.Name;
			second.Name = first.Name;

			int32 tempInt = first.NameLength;
			first.NameLength = second.NameLength;
			second.NameLength = tempInt;

			Texture2D *swapTex = first.AmbientMap;
			first.AmbientMap = second.AmbientMap;
			second.AmbientMap = swapTex;

			swapTex = first.DiffuseMap;
			first.DiffuseMap = second.DiffuseMap;
			second.DiffuseMap = swapTex;

			swapTex = first.SpecularMap;
			first.SpecularMap = second.SpecularMap;
			second.SpecularMap = swapTex;

			swapTex = first.GlossMap;
			first.GlossMap = second.GlossMap;
			second.GlossMap = swapTex;

			swapTex = first.DissolveMap;
			first.DissolveMap = second.DissolveMap;
			second.DissolveMap = swapTex;

			swapTex = first.BumpMap;
			first.BumpMap = second.BumpMap;
			second.BumpMap = swapTex;

			swapTex = first.NormalMap;
			first.NormalMap = second.NormalMap;
			second.NormalMap = swapTex;

			swapTex = first.ReflectionMap;
			first.ReflectionMap = second.ReflectionMap;
			second.ReflectionMap = swapTex;


			Vector3 swapVec3 = first.AmbientColor;
			first.AmbientColor = second.AmbientColor;
			second.AmbientColor = swapVec3;

			swapVec3 = first.DiffuseColor;
			first.DiffuseColor = second.DiffuseColor;
			second.DiffuseColor = swapVec3;

			swapVec3 = first.SpecularColor;
			first.SpecularColor = second.SpecularColor;
			second.SpecularColor = swapVec3;

			swapVec3 = first.TransmissionFilter;
			first.TransmissionFilter = second.TransmissionFilter;
			second.TransmissionFilter = swapVec3;


			real32 swapReal = first.Gloss;
			first.Gloss = second.Gloss;
			second.Gloss = swapReal;

			swapReal = first.Dissolve;
			first.Dissolve = second.Dissolve;
			second.Dissolve = swapReal;

			swapReal = first.ReflectionSharpness;
			first.ReflectionSharpness = second.ReflectionSharpness;
			second.ReflectionSharpness = swapReal;

			swapReal = first.OpticalDensity;
			first.OpticalDensity = second.OpticalDensity;
			second.OpticalDensity = swapReal;

			uint8 swapModel = first.IlluminationModel;
			first.IlluminationModel = second.IlluminationModel;
			second.IlluminationModel = swapModel;

			bool swapHalo = first.DissolveHalo;
			first.DissolveHalo = second.DissolveHalo;
			second.DissolveHalo = swapHalo;*/
		}

		Material() {}
		Material(char *name, int32 nameLength, Texture2D* diffuse) : Name(name), NameLength(nameLength), DiffuseMap(diffuse) { }

		Material(const Material &other) : NameLength(other.NameLength)
		{
			memcpy(this, &other, sizeof(Material));
			Name = CString::CopySubstring(Name, NameLength - 1);
		}

		Material(Material &&other) : Material()
		{
			swap(*this, other);
		}
		/**
			/brief Prepares the GL to draw meshes using this material
		*/
		void Use()
		{
			DiffuseMap->Bind();
		}

		Material &operator=(Material other)
		{
			swap(*this, other);
			return *this;
		}

		~Material()
		{
			delete[] Name;
			NameLength = 0;
			Name = nullptr;
		}
	};
}

#endif //HANDMADE_MATERIAL_H