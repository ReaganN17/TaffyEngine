
#include "TaffyEngine.h"
#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main() {
	//Win32Window::window_name = "testing";
	Win32Window::run(Win32Render::renderFillerBG, Win32Render::renderFillerBG);

}