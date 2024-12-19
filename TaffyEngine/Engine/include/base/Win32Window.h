#pragma once

#include "../utils/Utils.h"
namespace Win32Window {
	extern LPCWSTR window_name;
	extern HWND window;
	extern MONITORINFO mi;
	extern BITMAPINFO bitmap_info;
	extern int window_width, window_height, window_sizeInBits;
	extern bool running;

	extern float delta_time;

	LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void run(void (*init)(), void(*loop)());
	void end();
};