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

void SpriteHandler::handleSprites() {
	for (auto sprite : sprites) {
		for (auto it : ObjectHandler::getInstance().objects) {
			for (auto object : *it) {
				if (object->getSprite() == sprite.second) {
					break;
				}
				sprite.second->unload();
			}
		}
	}
}
