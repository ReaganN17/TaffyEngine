#include "TaffyEngine.h"


bool Win32Window::running;
const char* Win32Window::window_name;
HWND Win32Window::window;
MONITORINFO Win32Window::mi;
int Win32Window::window_width, Win32Window::window_height;
float Win32Window::delta_time;

void Win32Window::run(void (*init)(), void(*loop)()) {
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Window Class";
	window_class.lpfnWndProc = DefWindowProc;

	RegisterClass(&window_class);

	window = CreateWindow(window_class.lpszClassName, window_name, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

	mi = { sizeof(mi) };
	running = true;
	HDC hdc = GetDC(window);
	BITMAPINFO bitmap_info;

	float window_sizeInBits;

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

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {

			} break;

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
				TranslateMessage(&message);
				DispatchMessage(&message);

			}
			}
		}

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

void Win32Window::config(bool fullscreen, bool show_cursor) {
	ShowCursor(show_cursor);
	if (fullscreen) {
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void Win32Window::end(void (*end)()) {
	end();
	running = false;
}

	

