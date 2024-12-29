#pragma once

#include "../utils/Utils.h"
#include "../base/Win32Window.h"
#include "../base/Win32Render.h"

union Button_State {
	u8 byte = 0;
	struct {
		bool down : 1;
		bool changed : 1;
		u8 space : 6;
	};
};

/*
Input Class

Manages the keybaord and mouse inputs
*/
class Input final {
	friend Win32Window;

private:
	static Button_State buttons[255];
	static POINT mouse;

	//updates the key to not changed
	static void setKeyUnchanged();

	/**updates Mouse coordinates
	* 
	* @param Window Handle
	*/
	static void updateMouse(HWND window);

	/**
	* processes buttons and updates their status
	* 
	* @param vk key code
	* @param associated status
	*/
	static void processButtons(u32 vkcode, bool d);

	//Disables Construction
	Input() = delete;
public:
	/**
	* returns down status of button
	* 
	* @param button
	* @return is button down
	*/
	static bool is_down(u8 button);

	/**
	* returns pressed status of button
	*
	* @param button
	* @return was button pressed
	*/
	static bool is_pressed(u8 button);

	/**
	* returns released status of button
	*
	* @param button
	* @return was button released
	*/
	static bool is_released(u8 button);

	/**
	* returns a reference to the mouse position
	*
	* @return reference to a POINT struct
	*/
	static POINT& getMouse();
};