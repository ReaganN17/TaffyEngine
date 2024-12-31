#pragma once

#include "Utils.h"
/*
Shader Struct

Contains Shade and Opacity properties
*/
struct Shader final {
	u32 shade_color;
	u8 shade_scale, opacity;

	//Default constructor
	Shader();

	/**
	* Constructor
	* 
	* @param Opacity Scale 0 - 255
	* @param Shade color in Hexadecimal
	* @param Shade Scale 0 - 255
	*/
	Shader(u8 opac, u32 shade_color, u8 shade_scale);
};
