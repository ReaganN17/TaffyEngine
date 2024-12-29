#pragma once

#include "../utils/Utils.h"
#include "Win32Window.h"
#include "../handlers/objects/Object.h"
#include "EngineBase.h"
#include "Input.h"

/*
Win32Render Class

Contains window render methods and properties
*/
class Win32Render final {
	friend Win32Window;
	friend Object;
	friend EngineBase;
	friend Input;

private:
	static int screenWidth, screenHeight;
	static int screenOffset, yOffset, xOffset;
	static float render_scale, aspect_ratio;
	static int coord_height, coord_width;

	static void* memory;

	//Disables construction
	Win32Render() = delete;

	//Update screen values upon window creation and window movement
	static void updateValues();

	/**
	* clears ENTIRE screen, used for when window size changes
	* 
	* @param color to clear screen with
	*/
	static void clearEntireScreen(u32 color);

	/**
	* clears graphic screen (based on aspect ratio)
	*
	* @param color to clear screen with
	*/
	static void clearScreen(u32 color);

	/**
	* Renders a basic rectangle
	* 
	* @param x, y Location
	* @param w, h Dimensions
	* @param Color of Rectangle
	* @param Shader of Rectangle
	*/
	static void renderRect(float x, float y, float w, float h, u32 color, Shader& shade);

	/**
	* Renders an Image
	*
	* @param Image to rdner
	* @param x, y Location
	* @param w, h Dimensions
	* @param Shader of Rectangle
	*/
	static void renderImage(Image& img, float x, float y, float w, float h, Shader& shade);

public:
	/**
	* Returns the coordinate height of the render window
	* 
	* @return int value of the coordinate height
	*/
	static int getCoordHeight();

	/**
	* Returns the coordinate width of the render window
	* 
	* @return int value of the coordinate width
	*/
	static int getCoordWidth();

	//used for debugging, will delete soon
	static void draw_number(int number, float x, float y, float size);
	static void draw_digit(int digit, float x, float y, float size);

	//Renders a filler BG I use; delete soon
	static void renderFillerBG();
};