global_var float render_scale = 0.001f;

internal void renderStaticBG(const char* file, float scale, float alpha, u32 background) {
	Image img(file);

	alpha = clampF(0, alpha, 1);

	int size_x = img.w * scale;
	int size_y = img.h * scale;

	int x = renderWindow.width / 2.f;
	int y = renderWindow.height / 2.f;

	int x0 = x - (size_x / 2);
	int y0 = y - (size_y / 2);

	int offsetRight = max(0, x0 + size_x - renderWindow.width);
	int offsetLeft = max(0, -x0);

	int offsetBottom = max(0, -y0);
	int offsetTop = max(0, y0 + size_y - renderWindow.height);

	u32 src = 0;
	RGB curPixel;

	if (scale > 0) {

		for (int i = offsetTop; i < size_y - offsetBottom; i++) {
			u32* pixel = (u32*)renderWindow.memory + x0 + offsetLeft + ((size_y - 1) - i + y0) * renderWindow.width;
			for (int j = offsetLeft; j < size_x - offsetRight; j++) {
				src = img.channels * ((int)(j / scale) + img.w * (int)(i / scale));
				curPixel.r = img.data[src];
				curPixel.g = img.data[src + 1];
				curPixel.b = img.data[src + 2];

				*pixel++ = alphaPerPixel(background, RGBToHex(curPixel), alpha);
			}
		}
	}
}

internal void clearScreen(u32 color) {
	u32* pixel = (u32*)renderWindow.memory;
	for (int y = 0; y < renderWindow.height; y++) {
		for (int x = 0; x < renderWindow.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void renderBG(Camera cam) {
	u32* pixel = (u32*)renderWindow.memory;
	for (int y = 0; y < renderWindow.height; y++) {
		for (int x = 0; x < renderWindow.width; x++) {
			*pixel++ = x;
		}
	}
}


internal void renderRect(float x, float y, float half_size_x, float half_size_y, u32 color, float alpha, Camera cam) {
	x -= cam.x;
	y -= cam.y;

	x *= renderWindow.height * render_scale * cam.zoom;
	y *= renderWindow.height * render_scale * cam.zoom;

	half_size_x *= renderWindow.height * render_scale * cam.zoom;
	half_size_y *= renderWindow.height * render_scale * cam.zoom;

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

internal void renderImage(Image img, float x, float y, float wscale, float hscale, Camera cam, float alpha = 1) {
	alpha = clampF(0, alpha, 1);

	x -= cam.x;
	y -= cam.y;

	x *= renderWindow.height * render_scale * cam.zoom;
	y *= renderWindow.height * render_scale * cam.zoom;

	wscale *= renderWindow.height * render_scale * cam.zoom;
	hscale *= renderWindow.height * render_scale * cam.zoom;

	int size_x = img.w * wscale;
	int size_y = img.h * hscale;

	x += renderWindow.width / 2.f;
	y += renderWindow.height / 2.f;

	int x0 = x - (size_x / 2);
	int y0 = y - (size_y / 2);

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

/*

internal void
draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			renderRect(x - size, y, half_size, 2.5f * size, color);
			renderRect(x + size, y, half_size, 2.5f * size, color);
			renderRect(x, y + size * 2.f, half_size, half_size, color);
			renderRect(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			renderRect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color);
			renderRect(x, y, 1.5f * size, half_size, color);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color);
			renderRect(x + size, y + size, half_size, half_size, color);
			renderRect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color);
			renderRect(x - half_size, y, size, half_size, color);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color);
			renderRect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			renderRect(x + size, y, half_size, 2.5f * size, color);
			renderRect(x - size, y + size, half_size, 1.5f * size, color);
			renderRect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color);
			renderRect(x, y, 1.5f * size, half_size, color);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color);
			renderRect(x - size, y + size, half_size, half_size, color);
			renderRect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			renderRect(x + half_size, y + size * 2.f, size, half_size, color);
			renderRect(x + half_size, y, size, half_size, color);
			renderRect(x + half_size, y - size * 2.f, size, half_size, color);
			renderRect(x - size, y, half_size, 2.5f * size, color);
			renderRect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			renderRect(x + size, y, half_size, 2.5f * size, color);
			renderRect(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			renderRect(x - size, y, half_size, 2.5f * size, color);
			renderRect(x + size, y, half_size, 2.5f * size, color);
			renderRect(x, y + size * 2.f, half_size, half_size, color);
			renderRect(x, y - size * 2.f, half_size, half_size, color);
			renderRect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color);
			renderRect(x - half_size, y, size, half_size, color);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color);
			renderRect(x + size, y, half_size, 2.5f * size, color);
			renderRect(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	}
}

*/