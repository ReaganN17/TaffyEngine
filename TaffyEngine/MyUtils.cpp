#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>
#include <math.h>

using namespace std;

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned u32;
typedef long s64;
typedef unsigned long u64;

#define internal static
#define global_var static

//rendering variables

global_var int screenWidth = 960;
global_var int screenHeight = 540;
global_var int screenOffset = 0;
global_var int yOffset = 0;
global_var int xOffset = 0;
global_var float render_scale = (float) screenHeight / 540;

struct RenderWindow {
	int height, width, sizeInBits;
	u8 bitS;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_var RenderWindow renderWindow;

#include "MathUtils.cpp"

#include "RGBUtils.cpp"

#include "ImageUtils.cpp"

#include "CameraUtils.cpp"