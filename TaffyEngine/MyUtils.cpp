#include "TaffyEngine.h"

//Crop Info
CropInfo::CropInfo() : cx(0), cy(0), cw(0), ch(0){}

CropInfo::CropInfo(u16 x, u16 y, u16 w, u16 h) : cx(x), cy(y), cw(w), ch(h) {}

CropInfo& CropInfo::set(u16 x, u16 y, u16 w, u16 h) {cx = x, cy = y, cw = w, ch = h;}

//math and coding
#define arrContainsBool(element, arr, sizeOfE) find(arr, arr + sizeof(arr) / sizeOfE, element) != arr + sizeof(arr) / sizeOfE
#define vecContainsBool(element, vec) find(vec.begin(), vec.end(), element) != vec.end()

int math::clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

float math::clamp(float min, float val, float max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

#define ALPHA(rgb) (uint8_t)(rgb >> 24)
#define RED(rgb)   (uint8_t)(rgb >> 16)
#define GREEN(rgb) (uint8_t)(rgb >> 8)
#define BLUE(rgb)  (uint8_t)(rgb)

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

u32 RGBA::toHex(u32 back, u8 opac = 255) {
	if (a == 255) return 0 << 24 | r << 16 | g << 8 | b;
	if (a == 0) return back;

	return 0 << 24 | BLEND(back, r, a * opac / 255) << 16 | BLEND(back, g, a * opac / 255) << 8 | BLEND(back, b, a * opac / 255);
}

RGBA& RGBA::shade(u32 shade, u8 scale) {
	if (scale == 0) return *this;
	if (scale == 255) {RGBA::RGBA(shade); return *this;}
	r = BLEND(RED(shade), r, 255 - scale);
	g = BLEND(GREEN(shade), g, 255 - scale);
	b = BLEND(BLUE(shade), b, 255 - scale);

	return *this;
}

Shader::Shader(u8 opacity, u32 shade_color, u8 shade_scale) : opacity(opacity), shade_color(shade_color), shade_scale(shade_scale){}
#include "ImageUtils.cpp"