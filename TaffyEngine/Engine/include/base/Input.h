#pragma once

#include "../utils/Utils.h"

union Button_State {
	u8 byte = 0;
	struct {
		bool down : 1;
		bool changed : 1;
		u8 space : 6;
	};
};

namespace Input {
	extern Button_State buttons[255];
	extern POINT mouse;

	//updates the key to not changed
	void setKeyUnchanged();

	/**updates Mouse coordinates
	* 
	* @param Window Handle
	*/
	void updateMouse(HWND window);

	/**
	* processes buttons and updates their status
	* 
	* @param vk key code
	* @param associated status
	*/
	void processButtons(u32 vkcode, bool d);

	/**
	* returns down status of button
	* 
	* @param button
	* @return is button down
	*/
	bool is_down(u8 button);

	/**
	* returns pressed status of button
	*
	* @param button
	* @return was button pressed
	*/
	bool is_pressed(u8 button);

	/**
	* returns released status of button
	*
	* @param button
	* @return was button released
	*/
	bool is_released(u8 button);
};