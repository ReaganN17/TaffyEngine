#define STB_IMAGE_IMPLEMENTATION

#include "STBI_ImageUtils.cpp"

struct Image {

	u8* data = NULL;
	size_t size = 0;
	u16 w = 0, h = 0;
	u8 channels = 0;
	short rotation = 0;

	Image();
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);

	~Image();
	
	bool read(const char* filename);
	bool isPNG(const char* filename);

	Image& rotate(short rot);
	Image& rotateTo(short rot);	
	Image& copy(Image img);

	Image& grayscale_avg();
	Image& create(const char* filename);
	Image& crop(u16 cx, u16 cy, u16 cw, u16 ch);

	Image produceCrop(u16 cx, u16 cy, u16 cw, u16 ch);
	void produceCropImage(u16 cx, u16 cy, u16 cw, u16 ch, Image* img);
};
//End of Header

//start of class functions

Image::Image() {}

Image::Image(const char* filename) {
	if (read(filename)) {
		size = w * h * channels;
	}
	else {
		Image::Image(1, 1, 3);
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
	stbi_image_free(data);
	data = NULL;
}

bool Image::read(const char* filename) {
	rotation = 0;
	delete[] data;

	if (!isPNG(filename)) return false;

	data = stbi_load(filename, (int*)&w, (int*)&h, (int*)&channels, 0);

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

Image& Image::rotateTo(short rot) {
	rot /= 90;
	rot *= 90;
	rot = (rot + 180) % 360 - 180;
	
	if ((rot + 360) % 360 - 180 == rotation) { rotate(-180); }
	if ((rot + 270) % 360 - 180 == rotation) { rotate(-90); }
	if ((rot + 450) % 360 - 180 == rotation) { rotate(90);  }

	return *this;
}

Image& Image::rotate(short rot) {
	rot /= 90;
	rot *= 90;
	rot = (rot + 180) % 360 - 180;
	rotation += rot;
	rotation = (rotation + 540) % 360 - 180;

	u8* result = new u8[size];

	switch (rot) {
	case -180: {
		for (int i = 0; i < w * h; i++) {
			memcpy(&result[channels * (w * h - i - 1)], &data[channels * i], channels);
		}
	
	}break;
	case -90: {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				memcpy(&result[channels * ((h - 1- i) + j * h)], &data[channels * (j + i * w)], channels);
			}
		}
		w = w + h;
		h = w - h;
		w = w - h;
	
	}break;
	case 90: {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				memcpy(&result[channels * (i + (w - 1 - j) * h)], &data[channels * (j + i * w)], channels);
			}
		}
		w = w + h;
		h = w - h;
		w = w - h;
	
	}break;


	case 0: 
	default: delete[] result; rotation = 0; return *this;
	}

	delete[] data;
	data = result;
	result = nullptr;

	return *this;
}

Image& Image::grayscale_avg() {
	if (channels < 3) {

	}
	else {
		for (int i = 0; i < size; i += channels) {
			int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
			memset(data + i, gray, 3);
		}
	}
	return *this;
}

Image& Image::create(const char* filename) {
	if (read(filename)) {
		size = w * h * channels;
	}
	else {

	}
	return *this;
}

Image& Image::crop(u16 cx, u16 cy, u16 cw, u16 ch) {
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

Image Image::produceCrop(u16 cx, u16 cy, u16 cw, u16 ch) {
	Image cropped(cw, ch, channels);

	for (u16 y = 0; y < ch; ++y) {
		if (y + cy >= h) break;
		for (u16 x = 0; x < cw; ++x) {
			if (x + cx >= w) break;
			memcpy(&cropped.data[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}

	return cropped;
}

void Image::produceCropImage(u16 cx, u16 cy, u16 cw, u16 ch, Image* img) {
	img->w = cw;
	img->h = ch;
	img->channels = channels;
	img->size = channels * cw * ch;

	delete[] img->data;
	img->data = new u8[img->size];
	memset(img->data, 0, size);

	for (u16 y = 0; y < ch; ++y) {

		if (y + cy >= h) break;

		for (u16 x = 0; x < cw; ++x) {
			if (x + cx >= w) break;
			memcpy(&img->data[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}
}

Image& Image::copy(Image img) {
	w = img.w;
	h = img.h;
	channels = img.channels;
	size = w * h * channels;

	delete[] data;
	data = new u8[size];

	memset(data, 0, size);
	memcpy(&data, &img.data, size);

	return *this;
}



