#include "TaffyEngine.h"

Object rect(0, 0, 50, 100, 0xff00ff, Z_MIDDLE);

void init() {
	
}

void run() {
	if (Input::is_pressed(VK_ESCAPE)) { EngineBase::end(); }

}

int main() {

	EngineBase::run(init, run);

	return 0;
}