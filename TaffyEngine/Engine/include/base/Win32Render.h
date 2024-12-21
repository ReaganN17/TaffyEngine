#pragma once

#include "../utils/Utils.h"

namespace Win32Render {
	extern int screenWidth, screenHeight;
	extern int screenOffset, yOffset, xOffset;
	extern float render_scale, aspect_ratio;
	extern int coord_height, coord_width;

	extern void* memory;

	void updateValues();
	//clears ENTIRE screen, used for when window size changes
	void clearEntireScreen(u32 color);
	void clearScreen(u32 color);
	void renderStaticBG(Image& img, bool stretch, Shader shade);

	//render current BG that i use
	void renderFillerBG();

	//renders a rectangle
	void renderRect(float x, float y, float w, float h, u32 color, Shader& shade, Camera* camera);
	void renderImage(Image& img, float x, float y, float w, float h, Shader& shade, Camera* camera);

	//used for debugging, will delete soon
	void draw_number(int number, float x, float y, float size);
	void draw_digit(int digit, float x, float y, float size);
};