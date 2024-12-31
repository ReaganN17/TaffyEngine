#include "../../include/base/Win32Window.h"

bool Win32Window::running = false;

HWND Win32Window::window;
MONITORINFO Win32Window::mi;
BITMAPINFO Win32Window::bitmap_info;
int Win32Window::window_width, Win32Window::window_height, Win32Window::window_sizeInBits;
float Win32Window::delta_time = 0.016666f;

LPCWSTR Win32Window::window_name;

LRESULT CALLBACK Win32Window::window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {

	case WM_CLOSE:
	case WM_DESTROY: {

		running = false;
	} break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(window, &rect);

		window_width = rect.right - rect.left;
		window_height = rect.bottom - rect.top;
		window_sizeInBits = window_width * window_height * sizeof(unsigned int);

		if (Win32Render::memory) VirtualFree(Win32Render::memory, 0, MEM_RELEASE);
		Win32Render::memory = VirtualAlloc(0, window_sizeInBits, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
		bitmap_info.bmiHeader.biWidth = window_width;
		bitmap_info.bmiHeader.biHeight = window_height;
		bitmap_info.bmiHeader.biPlanes = 1;
		bitmap_info.bmiHeader.biBitCount = 32;
		bitmap_info.bmiHeader.biCompression = BI_RGB;

		Win32Render::updateValues();

	} break;

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}

	return result;
}

void Win32Window::run(void (*init)(), void(*loop)()) {
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Window Class";
	window_class.lpfnWndProc = window_callback;

	RegisterClass(&window_class);

	window = CreateWindow(window_class.lpszClassName, window_name, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

	ShowCursor(true);

	mi = { sizeof(mi) };
	SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
	GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
	SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);


	running = true;
	HDC hdc = GetDC(window);

	//frame calculator
	delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	LARGE_INTEGER perf;
	LARGE_INTEGER frame_end_time;

	QueryPerformanceCounter(&frame_begin_time);
	QueryPerformanceFrequency(&perf);
	float performance_frequency = (float)perf.QuadPart;
	init();



	while (running) {
		MSG message;
		
		Input::updateMouse();
		Input::setKeyUnchanged();

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				Input::processButtons((u32)message.wParam, ((message.lParam & (1 << 31)) == 0));
			} break;

			case WM_SIZE: {
				Win32Render::clearEntireScreen(0x000000);
			} break;

			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);

			}
			}
		}


		//TODO: figure out how to get rid of this
		Input::processMouseButtons(VK_LBUTTON);
		Input::processMouseButtons(VK_RBUTTON);
		Input::processMouseButtons(VK_MBUTTON);
		Input::processMouseButtons(VK_XBUTTON1);
		Input::processMouseButtons(VK_XBUTTON2);

		// Simulate
		loop();

		// Render
		StretchDIBits(hdc, 0, 0, window_width, window_height, 0, 0, window_width, window_height, Win32Render::memory, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		delta_time = 0;
		while (delta_time < 0.01) {
			QueryPerformanceCounter(&frame_end_time);
			delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		}
		frame_begin_time = frame_end_time;
	}
}

float Win32Window::getDT() {
	return delta_time;
}



