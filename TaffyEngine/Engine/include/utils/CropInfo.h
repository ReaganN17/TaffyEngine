#pragma once

#include "../utils/Utils.h"

/*
CropInfo Struct

Contains rectangle position and dimensions to crop an Image
*/
struct CropInfo final {
	u16 cx = 0;
	u16 cy = 0;
	u16 cw = 0;
	u16 ch = 0;

	//Default Constructor
	CropInfo();

	/**
	* Constructor
	* 
	* x, y position
	* width, height dimention
	*/
	CropInfo(u16 cx, u16 cy, u16 cw, u16 ch);
};