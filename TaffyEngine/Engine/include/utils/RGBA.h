#pragma once

#include "../utils/Utils.h"

struct RGBA final {
	u8 r, g, b, a;
	RGBA(u8 r, u8 g, u8 b, u8 a);
	RGBA(u8* imgdata, u32 index, u8 channels);
	RGBA(u32 hex);

	u32 toHex(u32 background, u8 opac);
	u32 toHex();
	RGBA& shade(u32 shade, u8 scale);

	//static u32 rgbToHex(u8 r, u8 g, u8 b, u8 a, u32 back);
};