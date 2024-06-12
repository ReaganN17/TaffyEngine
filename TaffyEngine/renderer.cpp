global_var float render_scale = 0.001f;

global_var int screenWidth;
global_var int screenHeight;
global_var int screenOffset;

//clears ENTIRE screen, used for when window size changes
internal void clearScreen(u32 color) {
	u32* pixel = (u32*)renderWindow.memory;
	for (int y = 0; y < renderWindow.height; y++) {
		for (int x = 0; x < renderWindow.width; x++) {
			*pixel++ = color;
		}
	}
}

//Maintain aspect ratio of 16/9, will add custom ratio later
internal void updateValues() {
	screenWidth = min(renderWindow.height * 16 / 9, renderWindow.width);
	screenHeight = min(renderWindow.width * 9 / 16, renderWindow.height);

	screenOffset = max((renderWindow.width - screenWidth) / 2, 0) + max((renderWindow.height - screenHeight) / 2, 0) * renderWindow.width;
}

//no matter the size will stretch the image to the boundaries
//image needs to be cropped or sized beforehand
internal void renderStaticBG(const char* file, float alpha, u32 background) {

	Image img(file);

	float wscale = ((float)screenWidth/img.w);
	float hscale = ((float)screenHeight/img.h);

	u32 src;
	RGB curPixel;


	if (wscale > 0 && hscale > 0) {
		for (int i = 0; i < screenHeight; i++) {
			u32* pixel = (u32*)renderWindow.memory + screenOffset + i * renderWindow.width;
			for (int j = 0; j < screenWidth; j++) {
				if (img.data == NULL) { *pixel++ = background; }
				else {
					src = img.channels * ((int)(j / wscale) + img.w * (int)((screenHeight - i - 1) / hscale));

					curPixel.r = img.data[src];
					curPixel.g = img.data[src + 1];
					curPixel.b = img.data[src + 2];

					*pixel++ = alphaPerPixel(background, RGBToHex(curPixel), alpha);
				}
			}
		}
	}
}

//render current BG that i use
internal void renderBG(Camera cam) {
	for (int i = 0; i < screenHeight; i++) {
		u32* pixel = (u32*)renderWindow.memory + screenOffset + i * renderWindow.width;
		for (int x = 0; x < screenWidth; x++) {
			*pixel++ = x;
		}
	}
}

//renders a rectangle
internal void renderRect(float x, float y, float half_size_x, float half_size_y, u32 color, float alpha, Camera cam) {
	x -= cam.x;
	y -= cam.y;

	x *= screenHeight * render_scale * cam.zoom;
	y *= screenHeight * render_scale * cam.zoom;

	half_size_x *= screenHeight * render_scale * cam.zoom;
	half_size_y *= screenHeight * render_scale * cam.zoom;

	x += screenWidth / 2.f;
	y += screenHeight / 2.f;

	// Change to pixel
	int x0 = clamp(0, x - half_size_x, screenWidth);
	int x1 = clamp(0, x + half_size_x, screenWidth);
	int y0 = clamp(0, y - half_size_y, screenHeight);
	int y1 = clamp(0, y + half_size_y, screenHeight);

	for (int i = y0; i < y1; i++) {
		u32* pixel = (u32*)renderWindow.memory + x0 + screenOffset + i * renderWindow.width;
		for (int j = x0; j < x1; j++) {
			*pixel++ = alphaPerPixel(*pixel, color, alpha);
		}
	}
}

//renders an image
internal void renderImage(Image img, float x, float y, float wscale, float hscale, Camera cam, float alpha = 1) {
	alpha = clampF(0, alpha, 1);

	x -= cam.x;
	y -= cam.y;

	x *= screenHeight * render_scale * cam.zoom;
	y *= screenHeight * render_scale * cam.zoom;

	wscale *= screenHeight * render_scale * cam.zoom;
	hscale *= screenHeight * render_scale * cam.zoom;

	int size_x = img.w * wscale;
	int size_y = img.h * hscale;

	x += screenWidth / 2.f;
	y += screenHeight / 2.f;

	int x0 = x - (size_x / 2);
	int y0 = y - (size_y / 2);

	int offsetRight = max(0, x0 + size_x - screenWidth);
	int offsetLeft = max(0, -x0);

	int offsetBottom = max(0, -y0);
	int offsetTop = max(0, y0 + size_y - screenHeight);

	u32 src = 0;
	RGB curPixel;

	if (wscale > 0 && hscale > 0) {

		for (int i = offsetTop; i < size_y - offsetBottom; i++) {
			u32* pixel = (u32*)renderWindow.memory + x0 + offsetLeft + screenOffset + ((size_y - 1) - i + y0) * renderWindow.width;
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


//used for debugging, will delete soon
internal void
draw_number(int number, float x, float y, float size) {
	float half_size = size * .5f;
	u32 color = 0xffffff;
	float alpha = 1;
	Camera cam(0, 0, 1);


	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x, y + size * 2.f, half_size, half_size, color, alpha, cam);
			renderRect(x, y - size * 2.f, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 1: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			x -= size * 2.f;
		} break;

		case 2: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color, alpha, cam);
			renderRect(x, y, 1.5f * size, half_size, color, alpha, cam);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color, alpha, cam);
			renderRect(x + size, y + size, half_size, half_size, color, alpha, cam);
			renderRect(x - size, y - size, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 3: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha, cam);
			renderRect(x - half_size, y, size, half_size, color, alpha, cam);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color, alpha, cam);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 4: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x - size, y + size, half_size, 1.5f * size, color, alpha, cam);
			renderRect(x, y, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 5: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color, alpha, cam);
			renderRect(x, y, 1.5f * size, half_size, color, alpha, cam);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color, alpha, cam);
			renderRect(x - size, y + size, half_size, half_size, color, alpha, cam);
			renderRect(x + size, y - size, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 6: {
			renderRect(x + half_size, y + size * 2.f, size, half_size, color, alpha, cam);
			renderRect(x + half_size, y, size, half_size, color, alpha, cam);
			renderRect(x + half_size, y - size * 2.f, size, half_size, color, alpha, cam);
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x + size, y - size, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 7: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 8: {
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x, y + size * 2.f, half_size, half_size, color, alpha, cam);
			renderRect(x, y - size * 2.f, half_size, half_size, color, alpha, cam);
			renderRect(x, y, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;

		case 9: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha, cam);
			renderRect(x - half_size, y, size, half_size, color, alpha, cam);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color, alpha, cam);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha, cam);
			renderRect(x - size, y + size, half_size, half_size, color, alpha, cam);
			x -= size * 4.f;
		} break;
		}

	}
}

