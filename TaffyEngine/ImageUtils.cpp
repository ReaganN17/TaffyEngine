#define STB_IMAGE_IMPLEMENTATION

#include "STBI_ImageUtils.cpp"

struct Image {

	u8* data = NULL;
	size_t size = 0;
	int w, h, channels;

	Image(const char* filename) {
		if (read(filename)) {
			size = w * h * channels;
		}
		else {

		}
	}

	Image(int w, int h, int channels) :w(w), h(h), channels(channels) {
		size = w * h * channels;
		data = new u8[size];
	}


	Image(const Image& img) :Image(img.w, img.h, img.channels) {
		memcpy(data, img.data, size);
	}

	~Image() {
		stbi_image_free(data);
		data = NULL;
	}

	Image() {} // default constructor
	


	bool read(const char* filename) {
		delete[] data;

		if (!isPNG(filename)) return false;

		data = stbi_load(filename, &w, &h, &channels, 0);

		if (channels < 3) {
			channels += 2;

			size = w * h * channels;
	
			u8* convert = new u8[size];
			memset(convert, 0, size);

			for (int i = 0; i < w * h; i++) {
				memcpy(&convert[(channels * i)], &data[(channels - 2) * i], 1);
				memcpy(&convert[(channels * i) + 1], &data[(channels - 2) * i], 1);
				memcpy(&convert[(channels * i) + 2], &data[(channels - 2) * i], 1);
				
				if (channels == 4) {
					memcpy(&convert[(channels * i) + 3], &data[(channels - 2) * i + 1], 1);
				}
			}

			delete[] data;
			data = convert;
			convert = nullptr;
		}

		return data != NULL;
	}


	bool isPNG(const char* filename) {
		const char* ext = strrchr(filename, '.');
		if (ext != nullptr) {
			if (strcmp(ext, ".png") == 0) {
				return true;
			}
		}
		return false;
	}

	Image& grayscale_avg() {
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

	Image& shade(u32 hex, float opac) {
		RGB curPixel;
		opac = clampF(0, opac, 1);
		for (int i = 0; i < w * channels; i += channels) {
			curPixel.r = data[i];
			curPixel.g = data[i + 1];
			curPixel.b = data[i + 2];

			curPixel = hexToRGB(alphaPerPixel(RGBToHex(curPixel), hex, opac));

			data[i] = curPixel.r;
			data[i + 1] = curPixel.g;
			data[i + 2] = curPixel.b;
		}

		return *this;
	}

	Image& create(const char* filename) {
		if (read(filename)) {
			size = w * h * channels;
		}
		else {

		}
		return *this;
	}


	Image& crop(u16 cx, u16 cy, u16 cw, u16 ch) {
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
};

internal int getWidth(const char* filename) {
	Image img(filename);
	img.~Image();
	return img.w;
}

internal int getHeight(const char* filename) {
	Image img(filename);
	img.~Image();
	return img.h;
}