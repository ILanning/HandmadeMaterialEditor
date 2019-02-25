#ifndef HANDMADE_CHANNELS
#define HANDMADE_CHANNELS

namespace Drawing
{
	///Bitflag list of channels that an image may have.
	enum class Channels
	{
		Default = 1,
		Red = 2,
		R = 2,
		Green = 4,
		G = 4,
		Blue = 8,
		B = 8,
		Alpha = 16,
		A = 16,
		Matte = 32,
		M = 32,
		Luminance = 64,
		L = 64,
		ZDepth = 128,
		Z = 128,
		RGB = 14,
		RGBA = 30,
	};

	inline Channels operator|(Channels a, Channels b)
	{
		return (Channels)((int32)(a) | (int32)(b));
	}
}

#endif