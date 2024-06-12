global_var float render_scale = 0.001f;

internal void clear_screen(u32 color) {
	unsigned int* pixel = (u32*)renderWindow.memory;
	for (int y = 0; y < renderWindow.height; y++) {
		for (int x = 0; x < renderWindow.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void renderCoolBG() {
	unsigned int* pixel = (u32*)renderWindow.memory;
	for (int y = 0; y < renderWindow.height; y++) {
		for (int x = 0; x < renderWindow.width; x++) {
			*pixel++ = x;
		}
	}
}

internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color, float alpha) {
	x *= renderWindow.height * render_scale;
	y *= renderWindow.height * render_scale;

	half_size_x *= renderWindow.height * render_scale;
	half_size_y *= renderWindow.height * render_scale;

	x += renderWindow.width / 2.f;
	y += renderWindow.height / 2.f;

	// Change to pixel
	int x0 = clamp(0, x - half_size_x, renderWindow.width);
	int x1 = clamp(0, x + half_size_x, renderWindow.width);
	int y0 = clamp(0, y - half_size_y, renderWindow.height);
	int y1 = clamp(0, y + half_size_y, renderWindow.height);

	for (int i = y0; i < y1; i++) {
		u32* pixel = (u32*)renderWindow.memory + x0 + i * renderWindow.width;
		for (int j = x0; j < x1; j++) {
			*pixel++ = alphaPerPixel(*pixel, color, alpha);
		}
	}
}

internal void drawImageFinal(Image img, float x, float y, float wscale, float hscale, float alpha) {
	alpha = clampF(0, alpha, 1);

	x *= renderWindow.height * render_scale;
	y *= renderWindow.height * render_scale;

	wscale *= renderWindow.height * render_scale;
	hscale *= renderWindow.height * render_scale;

	int size_x = img.w * wscale;
	int size_y = img.h * hscale;

	x += renderWindow.width / 2.f;
	y += renderWindow.height / 2.f;

	int x0 = x - (size_x/2);
	int y0 = y - (size_y/2);

	int offsetRight = max(0, x0 + size_x - renderWindow.width);
	int offsetLeft = max(0, -x0);

	int offsetBottom = max(0, -y0);
	int offsetTop = max(0, y0 + size_y - renderWindow.height);

	u32 src = 0;
	RGB curPixel;

	if (wscale > 0 && hscale > 0) {

		for (int i = offsetTop; i < size_y - offsetBottom; i++) {
			u32* pixel = (u32*)renderWindow.memory + x0 + offsetLeft + ((size_y - 1) - i + y0) * renderWindow.width;
			for (int j = offsetLeft; j < size_x - offsetRight; j++) {
				src = img.channels * ((int)(j / wscale) + img.w * (int)(i / hscale));

				curPixel.r = img.data[src];
				curPixel.g = img.data[src + 1];
				curPixel.b = img.data[src + 2];

				*pixel++ = alphaPerPixel(*pixel, RGBToHex(curPixel), ((img.channels == 4) ? img.data[src + 3] : 255) * alpha / 255);
			}
		}
	}
}