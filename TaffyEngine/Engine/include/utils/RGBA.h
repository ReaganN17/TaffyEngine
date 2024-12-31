#pragma once
#include "Utils.h"
/*
RGBA Struct

Contains RGB code with A opacity and manipulations
*/
struct RGBA final {
	u8 r = 0, g = 0, b = 0, a = 0;

	//Default Constructor
	RGBA();

	/**
	* Constructor
	* 
	* @param r, g, b 0 - 255 
	* @param a opacity 0 - 255
	*/
	RGBA(u8 r, u8 g, u8 b, u8 a);

	/**
	* Constructor from Image data
	* 
	* @param Image reference
	* @param array index
	*/
	RGBA(Image& image, u32 data_index);

	/**
	* Constructor from Hex
	* 
	* @param Hex Code
	*/
	RGBA(u32 hex);

	/**
	* Converts RGB values to Hex
	* 
	* @return Hex Code
	*/
	u32 toHex();

	/**
	* Conversts RGBA and Opacity values to Hex
	* 
	* @param Background color in Hexadecimal
	* @param Opacity scale 0 - 255
	* @return Result Hex Code
	*/
	u32 toHex(u32 background, u8 opac);
	
	/**
	* Shades the pixel to a color
	* 
	* @param Shade color in Hexadecimal
	* @Param Shade Scale from 0 - 255
	* @return Reference to self
	*/
	RGBA& shade(u32 shade, u8 scale);
};