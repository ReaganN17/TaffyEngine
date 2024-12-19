#include "../../include/utils/Utils.h"

#define ALPHA(rgb) (u8)(rgb >> 24)
#define RED(rgb)   (u8)(rgb >> 16)
#define GREEN(rgb) (u8)(rgb >> 8)
#define BLUE(rgb)  (u8)(rgb)

#define BLEND(back, front, alpha) (u8) (((front * alpha) + (back * (255 - alpha))) / 255)

//rgba class
RGBA::RGBA(u8 r, u8 g, u8 b, u8 a = 255) : r(r), g(g), b(b), a(a) {}

RGBA::RGBA(u32 hex) {
	a = hex >> 24;
	r = hex >> 16;
	g = hex >> 8;
	b = hex;
}

RGBA::RGBA(u8* ImgData, u32 src, u8 size) {
	r = (ImgData[src] & 0xff);
	g = (ImgData[((size > 2) ? 1 : 0) + src] & 0xff);
	b = (ImgData[((size > 2) ? 2 : 0) + src] & 0xff);
	a = (size % 2 == 0) ? ImgData[src + ((size > 2) ? 3 : 1)] : 255;
}

u32 RGBA::toHex(u32 back, u8 opac) {
	if (a == 255 && opac == 255) return 0 << 24 | r << 16 | g << 8 | b;
	if (a == 0 || opac == 0) return back;

	return 0 << 24 | BLEND(RED(back), r, a * opac / 255.f) << 16 | BLEND(GREEN(back), g, a * opac / 255.f) << 8 | BLEND(BLUE(back), b, a * opac / 255.f);
}

u32 RGBA::toHex() {
	return 0 << 24 | r << 16 | g << 8 | b;
}

RGBA& RGBA::shade(u32 shade, u8 scale) {
	if (scale == 0) return *this;

	if (scale == 255) {
		r = shade >> 16;
		g = shade >> 8;
		b = shade;
		return *this;
	}
	r = BLEND(RED(shade), r, (u8)(255 - scale));
	g = BLEND(GREEN(shade), g, (u8)(255 - scale));
	b = BLEND(BLUE(shade), b, (u8)(255 - scale));

	return *this;
}
