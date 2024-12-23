#include "../../include/TaffyEngine.h"

SpriteHandler SpriteHandler::instance;

SpriteHandler& SpriteHandler::getInstance() {
	return instance;
}

void SpriteHandler::addSprite(const char* sprite_name, Sprite* sprite) {
	sprites[sprite_name] = sprite;
}

void SpriteHandler::loadSprite(const char* sprite) {
	if (sprites[sprite] != nullptr) sprites[sprite]->load();
}
void SpriteHandler::loadSprite(Sprite* sprite) {
	if (sprite != nullptr) sprite->load();
}

void SpriteHandler::unloadSprite(const char* sprite) {
	if (sprites[sprite] != nullptr) sprites[sprite]->unload();
}
void SpriteHandler::unloadSprite(Sprite* sprite) {
	if (sprite != nullptr) sprite->unload();
}

Sprite* SpriteHandler::getSprite(const char* sprite_name) {
	return sprites[sprite_name];
}

//TODO: figure out how to not do this jank; possible combining objects into one list, etc
void SpriteHandler::handleSprites() {
	for (auto sprite : sprites) {
		bool double_break = false;
		for (auto it : ObjectHandler::getInstance().objects) {
			for (auto object : *it) {
				if (object->getSprite() == sprite.second) {
					double_break = true;
					break;
				}
			}
			if (double_break) break;
		}
		if (!double_break) sprite.second->unload();
	}
}
