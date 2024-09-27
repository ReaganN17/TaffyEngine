#pragma once
typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned u32;
typedef long s64;
typedef unsigned long u64;

#define internal static
#define global_var static


struct CropInfo {
	u16 cx;
	u16 cy;
	u16 cw;
	u16 ch;

	CropInfo();
	CropInfo(u16 cx, u16 cy, u16 cw, u16 ch);
	CropInfo& set(u16 x, u16 y, u16 w, u16 h);
};

struct RGBA {
	u8 r, g, b, a;
	RGBA(u8 r, u8 g, u8 b, u8 a);
	RGBA(u8* imgdata, u32 index, u8 channels);
	RGBA(u32 hex);

	u32 toHex(u32 background, u8 opac);
	RGBA& shade(u32 shade, u8 scale);

	static u32 rgbToHex(u8 r, u8 g, u8 b, u8 a, u32 back);
};

struct Shader {
	u32 shade_color;
	u8 shade_scale, opacity;
	Shader(u8 opac, u32 shade_color, u8 shade_scale);
};

struct Image {
	u8* data = NULL;
	size_t size = 0;
	u16 w = 0, h = 0;
	u8 channels = 0;
	short rotation = 0;

	inline Image();
	inline Image(const char* filename);
	inline Image(int w, int h, int channels);
	inline Image(const Image& img);
	inline ~Image();

	inline bool read(const char* filename);
	inline  bool isPNG(const char* filename);

	inline Image& create(const char* filename);
	inline Image& copy(Image& img);

	inline Image& crop(u16 cx, u16 cy, u16 cw, u16 ch);
	inline Image& crop(CropInfo ci);

	inline Image produceCrop(u16 cx, u16 cy, u16 cw, u16 ch);
	inline Image produceCrop(CropInfo ci);

	inline Image& rotateBy(short rot);
	inline Image& rotateTo(short rot);
	
	inline Image& combineTR(Image other);
	inline Image& combineBL(Image other);

	inline Image& grayscale_avg();
	inline Image& convertChannels(u8 channels);
};

namespace math {
	int clamp(int min, int val, int max);
	float clamp(float min, float val, float max);
}