#include "../../include/handlers/SpriteHandler.h"
#include "../../include/handlers/ObjectHandler.h"

std::map<const char*, Sprite*> SpriteHandler::sprites;
Timer SpriteHandler::handle_frequency;

void SpriteHandler::addSprite(const char* sprite_name, Sprite* sprite) {
	if (sprites[sprite_name] != nullptr) { sprites[sprite_name]->~Sprite(); }
	sprites[sprite_name] = sprite;
}

void SpriteHandler::updateSprite(const char* sprite_name, Sprite* sprite)
{
	std::vector<Object*> objects;

	for (auto it : ObjectHandler::objects) {
		for (auto object : *it) {
			if (object->getSprite() == getSprite(sprite_name)) {
				objects.push_back(object);
			}
		}
	}

	addSprite(sprite_name, sprite);

	for (Object* obj : objects) {
		obj->setSprite(sprite_name);
	}

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

void SpriteHandler::handleSprites(long frequency) {
	if (handle_frequency.hasElapsed(frequency)) {
		for (auto sprite : sprites) {
			bool double_break = false;
			for (auto it : ObjectHandler::objects) {
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

		handle_frequency.start();
	}
}
