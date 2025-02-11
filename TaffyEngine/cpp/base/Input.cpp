#include "../../include/base/Input.h"

Button_State Input::buttons[255];
POINT Input::mouse;

void Input::processButtons(u32 vk, bool d) {
	buttons[vk].down = d;
}

void Input::processMouseButtons(u32 vk) {
	buttons[vk].down = GetAsyncKeyState(vk) & 0x8000;
}

void Input::setKeyUnchanged() {
	for (int i = 0; i < 255; i++) {
		buttons[i].previous = buttons[i].down;
	}
}
void Input::updateMouse() {
	GetCursorPos(&mouse);
	ScreenToClient(Win32Window::window, &mouse);
	mouse.y = Win32Render::coord_height * 0.5f - (mouse.y - Win32Render::yOffset) * (Win32Render::coord_height / (float)Win32Render::screenHeight);
	mouse.x = (mouse.x - (Win32Window::window_width * 0.5f)) * (Win32Render::coord_height / (float)Win32Render::screenHeight);
	mouse.x = math::clamp(-Win32Render::coord_width * 0.5, (float)mouse.x, Win32Render::coord_width * 0.5f);
	mouse.y = math::clamp(-Win32Render::coord_height * 0.5, (float)mouse.y, Win32Render::coord_height * 0.5f);
}

bool Input::is_down(u8 button) {
	return buttons[button].down;
}
bool Input::is_pressed(u8 button) {
	return buttons[button].down && buttons[button].down != buttons[button].previous;
}
bool Input::is_released(u8 button) {
	return !buttons[button].down && buttons[button].down != buttons[button].previous;
}

POINT& Input::getMouse() {
	return mouse;
}