#pragma once

#include "../utils/Utils.h"

namespace Win32Render {
	extern int screenWidth, screenHeight;
	extern int screenOffset, yOffset, xOffset;
	extern float render_scale, aspect_ratio;
	extern int coord_height, coord_width;

	extern void* memory;

	//Update screen values upon window creation and window movement
	void updateValues();

	/**
	* clears ENTIRE screen, used for when window size changes
	* 
	* @param color to clear screen with
	*/
	void clearEntireScreen(u32 color);

	/**
	* clears graphic screen (based on aspect ratio)
	*
	* @param color to clear screen with
	*/
	void clearScreen(u32 color);

	/**
	* Renders an image to the corners of the screen; used for backgrounds
	* 
	* @param Image to render
	* @param Image shader
	*/
	void renderStaticBG(Image& img, Shader shade);

	//Renders a filler BG I use; delete soon
	void renderFillerBG();

	/**
	* Renders a basic rectangle
	* 
	* @param x, y Location
	* @param w, h Dimensions
	* @param Color of Rectangle
	* @param Shader of Rectangle
	*/
	void renderRect(float x, float y, float w, float h, u32 color, Shader& shade);

	/**
	* Renders an Image
	*
	* @param Image to rdner
	* @param x, y Location
	* @param w, h Dimensions
	* @param Shader of Rectangle
	*/
	void renderImage(Image& img, float x, float y, float w, float h, Shader& shade);

	//used for debugging, will delete soon
	void draw_number(int number, float x, float y, float size);
	void draw_digit(int digit, float x, float y, float size);
};