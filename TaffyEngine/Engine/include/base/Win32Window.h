#pragma once

#include "../utils/Utils.h"
namespace Win32Window {
	extern LPCWSTR window_name;
	extern HWND window;
	extern MONITORINFO mi;
	extern BITMAPINFO bitmap_info;
	extern int window_width, window_height, window_sizeInBits;
	extern bool running;

	//Change in time
	extern float delta_time;

	//Window Callback
	LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* Creates the Window and runs the renderer
	* 
	* @param Initialize
	* @param Loop
	*/
	void run(void (*init)(), void(*loop)());

	//Ends and destroys the Window
	void end();
};