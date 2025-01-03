#include "../../include/utils/RGBA.h"

#define ALPHA(rgb) (u8)(rgb >> 24)
#define RED(rgb)   (u8)(rgb >> 16)
#define GREEN(rgb) (u8)(rgb >> 8)
#define BLUE(rgb)  (u8)(rgb)

#define BLEND(back, front, alpha) (u8) (((front * alpha) + (back * (255 - alpha))) / 255)

RGBA::RGBA() {}

RGBA::RGBA(u8 r, u8 g, u8 b, u8 a = 255) : r(r), g(g), b(b), a(a) {}

RGBA::RGBA(u32 hex) {
	a = hex >> 24;
	r = hex >> 16;
	g = hex >> 8;
	b = hex;
}

RGBA::RGBA(Image& image, u32 data_index) {
	r = (image.data[data_index * image.channels] & 0xff);
	g = (image.data[((image.channels > 2) ? 1 : 0) + data_index * image.channels] & 0xff);
	b = (image.data[((image.channels > 2) ? 2 : 0) + data_index * image.channels] & 0xff);
	a = (image.channels % 2 == 0) ? image.data[data_index * image.channels + ((image.channels > 2) ? 3 : 1)] : 255;
}

u32 RGBA::toHex(u32 back, u8 opac) {
	if (a == 255 && opac == 255) return toHex();
	if (a == 0 || opac == 0) return back;

	return (u8)(a * opac / 255.f) << 24 | BLEND(RED(back), r, a * opac / 255.f) << 16 | BLEND(GREEN(back), g, a * opac / 255.f) << 8 | BLEND(BLUE(back), b, a * opac / 255.f);
}

u32 RGBA::toHex() {
	return a << 24 | r << 16 | g << 8 | b;
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
