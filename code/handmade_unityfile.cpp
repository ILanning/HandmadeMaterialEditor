#ifndef HANDMADE_UNITYFILE_CPP
#define HANDMADE_UNITYFILE_CPP

//TODO:  Do this via code generation from the Visual Studio project files
#include "content/AssetManager.cpp"
#include "content/MTLLoader.cpp"
#include "content/OBJLoader.cpp"
#include "content/ShaderManager.cpp"

#include "drawing/defaults/DefaultMaterials.cpp"
#include "drawing/GeometryHelpers.cpp"
#include "drawing/Texture2D.cpp"
#include "drawing/VertexColorTextureArray.cpp"
#include "drawing/VertexNormalTextureArray.cpp"

#include "general/memory/FixedSizeArena.cpp"
#include "general/memory/StackArena.cpp"
#include "general/HashMap.cpp"
#include "general/PathHelpers.cpp"

#include "math/Matrix3.cpp"
#include "math/Matrix4.cpp"
#include "math/Quaternion.cpp"
#include "math/SphericalCoord.cpp"
#include "math/Vector2.cpp"
#include "math/Vector3.cpp"

#include "handmade.cpp"
#include "library_implementations.cpp"
#include "PlatformGameSettings.cpp"
#include "TestScene.cpp"

#endif //HANDMADE_UNITYFILE_CPP