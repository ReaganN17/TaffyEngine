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


#define isdown(b) Input::buttons[b].down
#define pressed(b) (Input::buttons[b].down && Input::buttons[b].changed)
#define released(b) (!Input::buttons[b].down && Input::buttons[b].changed)

namespace Input {
	extern Button_State buttons[255];
	extern POINT mouse;

	void setKeyUnchanged();
	void updateMouse(HWND window);
	void processButtons(u32 vkcode, bool d);
};