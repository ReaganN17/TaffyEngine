#include "Engine/include/TaffyEngine.h"

#include "GameExample.cpp"

GameExample game;

int main() {
	Win32Window::window_name = L"Game Example";

	EngineBase::run([]() {game.init(); }, []() {game.play(); });

	return 0;
}