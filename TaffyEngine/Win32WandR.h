#pragma once

namespace Win32Window {
	extern const char* window_name;
	extern HWND window;
	extern MONITORINFO mi;
	extern int window_width, window_height;
	extern bool running;

	extern float delta_time;

	void run(void (*init)(), void(*loop)());
	void end(void (*init)());
	void config(bool fscreen, bool cursor);
};

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
	void renderRect(float x, float y, float w, float h, u32 color, Shader shade);
	void renderImage(Image& img, float x, float y, float w, float h, Shader shade);

	//used for debugging, will delete soon
	void draw_number(int number, float x, float y, float size);
};



