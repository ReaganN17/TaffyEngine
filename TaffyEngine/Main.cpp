#include "Engine/include/TaffyEngine.h"

#include "GameExample.cpp"

GameExample game;

int main() {
	EngineBase::run([]() {game.init(); }, []() {game.play(); });

	return 0;
}