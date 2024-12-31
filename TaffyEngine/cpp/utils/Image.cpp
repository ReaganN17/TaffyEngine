#define STB_IMAGE_IMPLEMENTATION

#include "../../include/utils/Image.h"
#include "STBI_image.cpp"

Image::Image() { }

Image::Image(const char* filename) {
	loadImage(filename);
}

Image::Image(const char* filename, CropInfo ci) {
	if (loadImage(filename)) {
		crop(ci);
	}
}

Image::Image(int w, int h, int channels) :w(w), h(h), channels(channels) {
	size = w * h * channels;
	data = new u8[size];
	memset(data, 0, size);
}

Image::Image(const Image& img) :Image(img.w, img.h, img.channels) {
	memcpy(data, img.data, size);
}

Image::~Image() {
	unloadImage();
}

bool Image::loadImage(const char* filename) {
	delete[] data;

	if (!isPNG(filename)) return false;

	data = stbi_load(filename, (int*)&w, (int*)&h, (int*)&channels, 0);

	size = w * h * channels;

	return data != NULL;
}

bool Image::isPNG(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (ext != nullptr) {
		if (strcmp(ext, ".png") == 0) {
			return true;
		}
	}
	return false;
}


bool Image::unloadImage() {
	if (data == NULL) return false;

	stbi_image_free(data);
	data = NULL;
	delete[] data;

	return true;
}

Image& Image::crop(u16 cx, u16 cy, u16 cw, u16 ch) {
	cw = min(cw, w - cx);
	ch = min(ch, h - cy);
	size = cw * ch * channels;
	u8* croppedImage = new u8[size];
	memset(croppedImage, 0, size);

	for (u16 y = 0; y < ch; ++y) {
		if (y + cy >= h) break;
		for (u16 x = 0; x < cw; ++x) {
			if (x + cx >= w) break;
			memcpy(&croppedImage[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}
	w = cw;
	h = ch;

	delete[] data;
	data = croppedImage;
	croppedImage = nullptr;

	return *this;
}

Image& Image::crop(CropInfo ci) {
	return crop(ci.cx, ci.cy, ci.cw, ci.ch);}

Image& Image::grayscale_avg() {
	if (channels > 2) {
		for (int i = 0; i < size; i += channels) {
			int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
			memset(data + i, gray, 3);
		}
	}
	return *this;
}