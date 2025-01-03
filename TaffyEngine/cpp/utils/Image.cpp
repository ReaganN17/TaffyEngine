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
	memset(data, 0 , size);
}

Image::Image(int w, int h, int channels, u8* data) :w(w), h(h), channels(channels)
{
	this->data = data;
	size = w * h * channels;
	
}

Image::Image(const Image& img) :Image(img.w, img.h, img.channels) {
	memcpy(data, img.data, size);
}

Image::~Image() {
	unloadImage();
}
void Image::createEmpty(int w, int h, int channels)
{
	delete[] data;

	this->w = w; this->h = h; this->channels = channels;
	size = w * h * channels;

	data = new u8[size];
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

Image& Image::convertChannels(u8 new_channels)
{
	if (channels == new_channels) return *this;

	u8* conversion = new u8[w * h * new_channels];

	//Uncovential but fast time complexity wise
	switch (new_channels) {
	case 1: {
		grayscale_avg();
		for (int i = 0; i < w * h; i++) {
			memset(&conversion[i * new_channels], data[i * channels], 1);
		}
	} break;
	case 2: {
		grayscale_avg();
		for (int i = 0; i < w * h; i++) {
			memset(&conversion[i * new_channels], data[i * channels], 1);
			memset(&conversion[i * new_channels + 1], (channels == 4)?data[i * channels + 3] : 255, 1);
		}
	} break;
	case 3: {
		for (int i = 0; i < w * h; i++) {
			if (channels != 4) { 
				memset(&conversion[i * new_channels], data[i * channels], 3); 
			}
			else
			{
				memcpy(&conversion[i * new_channels], &data[i * channels], 3);
			}

		}
	} break;
	case 4: {
		for (int i = 0; i < w * h; i++) {
			if (channels != 3) {
				memset(&conversion[i * new_channels], data[i * channels], 3);
			}
			else
			{
				memcpy(&conversion[i * new_channels], &data[i * channels], 3);
			}
			memset(&conversion[i * new_channels + 3], (channels == 2) ? data[i * channels + 1] : 255, 1);
		}
	} break;
	}

	delete[] data;
	data = conversion;
	conversion = nullptr;
	channels = new_channels;
	size = w * h * channels;

	return *this;
}

Image& Image::replace(u32 to_replace, u32 replacement) {
	convertChannels(4);
	if (channels != 4) { return *this; }

	for (int i = 0; i < w * h; i++) {
		RGBA pixel(*this, i);

		if (pixel.toHex() == to_replace) {
			RGBA rep(replacement);

			data[i * channels] = rep.r;
			data[i * channels + 1] = rep.g;
			data[i * channels + 2] = rep.b;
			data[i * channels + 3] = rep.a;
		}
	}

	return *this;
}

bool Image::combineTR(Image& image) {
	if (channels != image.channels) return false;

	u16 combined_h = max(h, image.h);
	u16 combined_w = (w + image.w);

	u8* combined_data = new u8[combined_w * combined_h * channels];
	memset(combined_data, 0, combined_w * combined_h * channels);

	for (int y = 0; y < combined_h; y++) {
		if (y < h) { 
			memcpy(&combined_data[y * combined_w * channels], &data[y * w * channels], w * channels); 
		}
		if (y < image.h) { 
			memcpy(&combined_data[y * combined_w * channels + w * channels], &image.data[y * image.w * channels], image.w * channels); 
		}
	}
	delete[] data;
	data = combined_data;
	combined_data = nullptr;

	w = combined_w, h = combined_h;
	size = w * h * channels;

	return true;
}

bool Image::combineBL(Image& image) {
	if (channels != image.channels) return false;

	u16 combined_h = (h + image.h);
	u16 combined_w = max(w, image.w);

	u8* combined_data = new u8[combined_w * combined_h * channels];
	memset(combined_data, 0, combined_w * combined_h * channels);

	for (int y = 0; y < combined_h; y++) {
		if (y < h) {
			memcpy(&combined_data[y * combined_w * channels], &data[y * w * channels], w * channels);
		}
		if (y >= h) {
			memcpy(&combined_data[y * combined_w * channels], &image.data[(y - h) * image.w * channels], image.w * channels);
		}
	}

	delete[] data;
	data = combined_data;
	combined_data = nullptr;

	w = combined_w, h = combined_h;
	size = w * h * channels;

	return true;
}

