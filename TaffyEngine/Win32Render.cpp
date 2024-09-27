#include "TaffyEngine.h"

int Win32Render::screenWidth, Win32Render::screenHeight;
int Win32Render::screenOffset, Win32Render::yOffset, Win32Render::xOffset;
float Win32Render::render_scale, Win32Render::aspect_ratio = 16 / 9.f;
int Win32Render::coord_height = 540, Win32Render::coord_width;
void* Win32Render::memory;

void Win32Render::updateValues() {
	coord_width = coord_height * aspect_ratio;
	screenWidth = min(Win32Window::window_height * aspect_ratio, Win32Window::window_width);
	screenHeight = min(Win32Window::window_width * (1/aspect_ratio), Win32Window::window_height);
	xOffset = max((Win32Window::window_width - screenWidth) / 2, 0);
	yOffset = max((Win32Window::window_height - screenHeight) / 2, 0);
	screenOffset = xOffset + yOffset * Win32Window::window_width;
	render_scale = (float)screenHeight / coord_height;	
	
}

//clears ENTIRE screen, used for when window size changes
void Win32Render::clearEntireScreen(u32 color) {
	u32* pixel = (u32*) memory;
	for (int y = 0; y < Win32Window::window_height; y++) {
		for (int x = 0; x < Win32Window::window_width; x++) {
			*pixel++ = color;
		}
	}
}

//clears screen, used for filler
void Win32Render::clearScreen(u32 color) {
	for (int y = 0; y < screenHeight; y++) {
		u32* pixel = (u32*)memory + screenOffset + y * Win32Window::window_width;
		for (int x = 0; x < screenWidth; x++) {
			*pixel++ = color;
		}
	}
}


//no matter the size will stretch the image to the boundaries
//image needs to be cropped or sized beforehand

void Win32Render::renderStaticBG(Image& img, bool stretch, Shader shade) {
	float wscale = ((float)screenWidth / img.w);
	float hscale = ((float)screenHeight / img.h);

	u32 src;

	if (wscale > 0 && hscale > 0) {
		for (int i = 0; i < screenHeight; i++) {
			u32* pixel = (u32*)memory + screenOffset + i * Win32Window::window_width;
			for (int j = 0; j < screenWidth; j++) {
				if (img.data == NULL) { *pixel++ = shade.shade_color; }
				else {
					src = img.channels * ((int)(j / wscale) + img.w * (int)((screenHeight - i - 1) / hscale));

					RGBA curPixel(img.data, src, img.channels);

					*pixel++ = curPixel.shade(shade.shade_color, shade.shade_scale).toHex(*pixel, shade.opacity);
				}
			}
		}
	}
}

void Win32Render::renderFillerBG() {
	for (int i = 0; i < screenHeight; i++) {
		u32* pixel = (u32*)memory + screenOffset + i * Win32Window::window_width;
		for (int x = 0; x < screenWidth; x++) {
			*pixel++ = x;
		}
	}
}

//render rectangle
void Win32Render::renderRect(float x, float y, float w, float h, u32 color, Shader shade) {

	x *= render_scale;
	y *= render_scale;

	w *= 0.5 * render_scale;
	h *= 0.5 * render_scale;

	x += screenWidth / 2.f;
	y += screenHeight / 2.f;

	// Change to pixel
	int x0 = math::clamp(0, (int)(x - w), screenWidth);
	int x1 = math::clamp(0, (int)(x + w), screenWidth);
	int y0 = math::clamp(0, (int)(y - h), screenHeight);
	int y1 = math::clamp(0, (int)(y + h), screenHeight);

	for (int i = y0; i < y1; i++) {
		u32* pixel = (u32*)memory + x0 + screenOffset + i * Win32Window::window_width;
		for (int j = x0; j < x1; j++) {
			RGBA curPixel(color);

			*pixel++ = curPixel.shade(shade.shade_color, shade.shade_scale).toHex(*pixel, shade.opacity);
		}
	}
}

void Win32Render::renderImage(Image& img, float x, float y, float w, float h, Shader shade) {

	int size_x = w * render_scale;
	int size_y = h * render_scale;

	float wscale = ((float)size_x) / img.w;
	float hscale = ((float)size_y) / img.h;

	int x0 = (x * render_scale) + 0.5 * (screenWidth - size_x);
	int y0 = (y * render_scale) + 0.5 * (screenHeight - size_y);

	int offsetRight = max(0, x0 + size_x - screenWidth);
	int offsetLeft = max(0, -x0);

	int offsetBottom = max(0, -y0);
	int offsetTop = max(0, y0 + size_y - screenHeight);

	u32 src = 0;

	if (wscale > 0 && hscale > 0) {
		for (int i = offsetTop; i < size_y - offsetBottom; i++) {
			u32* pixel = (u32*)memory + x0 + offsetLeft + screenOffset + ((size_y - 1) - i + y0) * Win32Window::window_width;
			for (int j = offsetLeft; j < size_x - offsetRight; j++) {

				src = img.channels * ((int)(j / wscale) + img.w * (int)(i / hscale));

				RGBA curPixel(img.data, src, img.channels);

				*pixel++ = curPixel.shade(shade.shade_color, shade.shade_scale).toHex(*pixel, shade.opacity);
			}
		}
	}
}


//used for debugging, will delete soon
void Win32Render::draw_number(int number, float x, float y, float size) {
	float half_size = size;
	u32 color = 0xff0000;
	Shader alpha(255, 0, 0);


	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x, y + size * 2.f, half_size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 1: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			x -= size * 2.f;
		} break;

		case 2: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x, y, 1.5f * size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x + size, y + size, half_size, half_size, color, alpha);
			renderRect(x - size, y - size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 3: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha);
			renderRect(x - half_size, y, size, half_size, color, alpha);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			x -= size * 4.f;
		} break;

		case 4: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x - size, y + size, half_size, 1.5f * size, color, alpha);
			renderRect(x, y, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 5: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x, y, 1.5f * size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x - size, y + size, half_size, half_size, color, alpha);
			renderRect(x + size, y - size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 6: {
			renderRect(x + half_size, y + size * 2.f, size, half_size, color, alpha);
			renderRect(x + half_size, y, size, half_size, color, alpha);
			renderRect(x + half_size, y - size * 2.f, size, half_size, color, alpha);
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x + size, y - size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 7: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 8: {
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x, y + size * 2.f, half_size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, half_size, half_size, color, alpha);
			renderRect(x, y, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 9: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha);
			renderRect(x - half_size, y, size, half_size, color, alpha);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x - size, y + size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;
		}

	}
}

