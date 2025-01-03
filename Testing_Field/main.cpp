#include "TaffyEngine.h"

Object rect;

void init() {
	SpriteHandler::addSprite("Text_1", new TextSprite("taffy's world"));
	SpriteHandler::addSprite("bobob", new Sprite("Resources/Bob.png"));

	new (&rect) Object(0, 0, 200, 50, "Text_1", Z_MIDDLE);
}

void run() {
	if (Input::is_pressed(VK_ESCAPE)) { EngineBase::end(); }

}

int main() {

	EngineBase::run(init, run);

	return 0;
}