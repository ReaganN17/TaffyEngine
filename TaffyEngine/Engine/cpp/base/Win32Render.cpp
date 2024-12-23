#include "../../include/TaffyEngine.h"

int Win32Render::screenWidth, Win32Render::screenHeight;
int Win32Render::screenOffset, Win32Render::yOffset, Win32Render::xOffset;
float Win32Render::render_scale, Win32Render::aspect_ratio = 16 / 9.f;
int Win32Render::coord_height = 540, Win32Render::coord_width;
void* Win32Render::memory;

void Win32Render::updateValues() {
	coord_width = coord_height * aspect_ratio;
	screenWidth = min(Win32Window::window_height * aspect_ratio, Win32Window::window_width);
	screenHeight = min(Win32Window::window_width * (1 / aspect_ratio), Win32Window::window_height);
	xOffset = max((Win32Window::window_width - screenWidth) / 2, 0);
	yOffset = max((Win32Window::window_height - screenHeight) / 2, 0);
	screenOffset = xOffset + yOffset * Win32Window::window_width;
	render_scale = (float)screenHeight / coord_height;

}

//clears ENTIRE screen, used for when window size changes
void Win32Render::clearEntireScreen(u32 color) {
	u32* pixel = (u32*)memory;
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
				if (img.data == NULL) {
					*pixel++ = 0x000000;
				}
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
void Win32Render::renderRect(float x, float y, float w, float h, u32 color, Shader& shade, Camera* camera = &Camera::default_camera) {
	if (!camera->isActivated()) return;
	x = (x - camera->getX()) * camera->getZoom();
	y = (y - camera->getY()) * camera->getZoom();
	w *= camera->getZoom();
	h *= camera->getZoom();

	int size_x = w * render_scale;
	int size_y = h * render_scale;

	int x0 = (x * render_scale) + 0.5 * (screenWidth - size_x);
	int y0 = (y * render_scale) + 0.5 * (screenHeight - size_y);

	int offsetRight = max(0, x0 + size_x - screenWidth);
	int offsetLeft = max(0, -x0);

	int offsetBottom = max(0, -y0);
	int offsetTop = max(0, y0 + size_y - screenHeight);

	for (int i = offsetTop; i < size_y - offsetBottom; i++) {
		u32* pixel = (u32*)memory + x0 + offsetLeft + screenOffset + ((size_y - 1) - i + y0) * Win32Window::window_width;
		for (int j = offsetLeft; j < size_x - offsetRight; j++) {
			*pixel++ = RGBA(255 << 24 | color).shade(shade.shade_color, shade.shade_scale).toHex(*pixel, shade.opacity);
		}
	}
}

void Win32Render::renderImage(Image& img, float x, float y, float w, float h, Shader& shade, Camera* camera = &Camera::default_camera) {
	if (!camera->isActivated() || img.data == NULL) return;
	x = (x - camera->getX()) * camera->getZoom();
	y = (y - camera->getY()) * camera->getZoom();
	w *= camera->getZoom();
	h *= camera->getZoom();

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
				*pixel++ = RGBA(img.data, src, img.channels).shade(shade.shade_color, shade.shade_scale).toHex(*pixel, shade.opacity);
			}
		}
	}
}


//used for debugging, will delete soon
void Win32Render::draw_number(int number, float x, float y, float size) {

	while (number != 0) {

		int digit = number % 10;
		number /= 10;

		draw_digit(digit, x + 0.8 * size * ((int) log10(10 * number + 1)), y, size);

	}
}

void Win32Render::draw_digit(int digit, float x, float y, float size) {
	u32 color = 0xff0000;
	Shader alpha(255, 0, 0);

	digit %= 10;
	if (digit != 1) renderRect(x - size * 0.2, y + size * 0.4, size * 0.2, size * 0.2, color, alpha);
	if (digit != 1 && digit != 4) renderRect(x, y + size * 0.4, size * 0.2, size * 0.2, color, alpha);
	renderRect(x + size * 0.2, y + size * 0.4, size * 0.2, size * 0.2, color, alpha);

	if ((digit ==0 || digit >= 4) && digit != 7) renderRect(x - size * 0.2, y + size * 0.2, size * 0.2, size * 0.2, color, alpha);
	if (digit != 5 && digit != 6) renderRect(x + size * 0.2, y + size * 0.2, size * 0.2, size * 0.2, color, alpha);

	if (digit != 1 && digit != 7) renderRect(x - size * 0.2, y, size * 0.2, size * 0.2, color, alpha);
	if (digit >= 2 && digit != 7) renderRect(x, y, size * 0.2, size * 0.2, color, alpha);
	renderRect(x + size * 0.2, y, size * 0.2, size * 0.2, color, alpha);

	if (digit % 2 == 0 && digit != 4) renderRect(x - size * 0.2, y - size * 0.2, size * 0.2, size * 0.2, color, alpha);
	if (digit != 2) renderRect(x + size * 0.2, y - size * 0.2, size * 0.2, size * 0.2, color, alpha);

	if (digit %3 != 1) renderRect(x - size * 0.2, y - size * 0.4, size * 0.2, size * 0.2, color, alpha);
	if (digit % 3 != 1) renderRect(x, y - size * 0.4, size * 0.2, size * 0.2, color, alpha);
	renderRect(x + size * 0.2, y - size * 0.4, size * 0.2, size * 0.2, color, alpha);
}

