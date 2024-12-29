#pragma once

#include "../utils/Utils.h"
#include "Win32Render.h"
#include "EngineBase.h"

/*
Win32Window Class

Contains Window methods and properites
*/
class Win32Window final {
	friend Win32Render;
	friend Input;
	friend EngineBase;

private:
	static HWND window;
	static MONITORINFO mi;
	static BITMAPINFO bitmap_info;
	static int window_width, window_height, window_sizeInBits;
	static bool running;
	static float delta_time;

	//Window Callback
	static LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* Creates the Window and runs the renderer
	*
	* @param Initialize
	* @param Loop
	*/
	static void run(void (*init)(), void(*loop)());

	//Disables construction
	Win32Window() = delete;
public:
	static LPCWSTR window_name;

	//Ends and destroys the Window
	static void end();

	/**
	* Returns time between frames
	* Used for consistent object movement
	* 
	* @return float value of change in time in seconds
	*/
	static float getDT();
};