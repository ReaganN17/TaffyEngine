#include "../../include/TaffyEngine.h"

Button_State Input::buttons[255];
POINT Input::mouse;

void Input::processButtons(u32 vk, bool d) {
	buttons[vk].changed = d != buttons[vk].down;
	buttons[vk].down = d;
}

void Input::setKeyUnchanged() {
	//note for later - figure out why "for each in" loop doesnt work and this one does
	for (int i = 0; i < 255; i++) {
		buttons[i].changed = false;
	}
}
void Input::updateMouse(HWND window) {
	GetCursorPos(&mouse);
	ScreenToClient(window, &mouse);
	mouse.y = Win32Render::coord_height * 0.5f - (mouse.y - Win32Render::yOffset) * (Win32Render::coord_height / (float)Win32Render::screenHeight);
	mouse.x = (mouse.x - (Win32Window::window_width * 0.5f)) * (Win32Render::coord_height / (float)Win32Render::screenHeight);
	mouse.x = math::clamp(-Win32Render::coord_width * 0.5, (float)mouse.x, Win32Render::coord_width * 0.5f);
	mouse.y = math::clamp(-Win32Render::coord_height * 0.5, (float)mouse.y, Win32Render::coord_height * 0.5f);
}