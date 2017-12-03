#ifndef HANDMADE_MTLTEXTUREOPTIONS
#define HANDMADE_MTLTEXTUREOPTIONS

#include "../drawing/Channels.h"

namespace Content
{
	namespace OBJ
	{
		struct MTLTextureOptions
		{
			char *TexturePath;
			int32 PathLength;

			//NOTE(Ian): Currently unsupported
			bool BlendU = true;
			//NOTE(Ian): Currently unsupported
			bool BlendV = true;
			//NOTE(Ian): Currently unsupported
			bool ColorCorrection = false;
			bool Clamp = false;

			float BumpMultiplier = 1;

			Vector2 ColorMod = { 0, 1 };
			Vector3 Offset = { 0, 0, 0 };
			Vector3 Scale = { 1, 1, 1 };
			//NOTE(Ian): Currently unsupported
			Vector3 Turbulence = { 0, 0, 0 };

			Drawing::Channels RawImageChannels = Drawing::Channels::Default;
			Drawing::Channels FinalChannels = Drawing::Channels::Default;
		};
	}
}

#endif