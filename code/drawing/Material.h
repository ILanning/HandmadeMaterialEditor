#ifndef HANDMADE_MATERIAL
#define HANDMADE_MATERIAL

#include "../handmade_typedefs.h"
#include "../math/Vector3.h"
#include "Texture2D.h"

struct Material
{
	char *Name;
	int32 NameLength;
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

	Material() {}
	Material(char *name, int32 nameLength, Texture2D* diffuse) : Name(name), NameLength(nameLength), DiffuseMap(diffuse) { }

	/**
		/brief Prepares the GL to draw meshes using this material
	*/
	void Use()
	{

	}

	~Material()
	{
		delete[] Name;
		NameLength = 0;
		Name = nullptr;
	}
};

#endif