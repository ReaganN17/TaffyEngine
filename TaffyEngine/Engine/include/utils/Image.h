#pragma once
#include "../utils/Utils.h"

struct Image final {
	u8* data = NULL;
	size_t size = 0;
	u16 w = 0, h = 0;
	u8 channels = 0;
	short rotation = 0;

	Image();
	Image(const char* filename);
	Image(const char* filename, CropInfo ci);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool isPNG(const char* filename);

	Image& create(const char* filename);
	Image& copy(const Image& img);

	Image& crop(u16 cx, u16 cy, u16 cw, u16 ch);
	Image& crop(CropInfo ci);

	Image produceCrop(u16 cx, u16 cy, u16 cw, u16 ch);
	Image produceCrop(CropInfo ci);

	Image& rotateBy(short rot);
	Image& rotateTo(short rot);
	
	Image& combineTR(Image other);
	Image& combineBL(Image other);

	Image& grayscale_avg();
	Image& convertChannels(u8 channels);

	Image& unloadData();
};