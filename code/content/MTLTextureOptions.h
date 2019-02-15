#ifndef HANDMADE_MTLTEXTUREOPTIONS_H
#define HANDMADE_MTLTEXTUREOPTIONS_H

#include "../handmade_typedefs.h"
#include "../drawing/Channels.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../general/HMString.h"

namespace Content
{
	namespace OBJ
	{
		///The options associated with a texture in a mtl file.
		struct MTLTextureOptions
		{
			///The path to the texture.
			HMString Path;

			//NOTE: Currently unsupported
			bool BlendU = true;
			//NOTE: Currently unsupported
			bool BlendV = true;
			//NOTE: Currently unsupported
			bool ColorCorrection = false;
			bool Clamp = false;

			real32 BumpMultiplier = 1;

			Vector2 ColorMod = { 0, 1 };
			Vector3 Offset = { 0, 0, 0 };
			Vector3 Scale = { 1, 1, 1 };
			//NOTE: Currently unsupported
			Vector3 Turbulence = { 0, 0, 0 };

			///The color channels in the image on disk
			Drawing::Channels RawImageChannels = Drawing::Channels::Default;
			///The color channels the image should have after loading
			Drawing::Channels FinalChannels = Drawing::Channels::Default;
		};
	}
}

#endif //HANDMADE_MTLTEXTUREOPTIONS_H