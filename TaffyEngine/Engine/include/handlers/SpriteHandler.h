#pragma once

#include "../utils/Utils.h"
struct SpriteHandler {
private:
	std::map<const char*, Sprite*> sprites;
	static SpriteHandler instance;

	SpriteHandler() {}
public:
	static SpriteHandler& getInstance();

	void addSprite(const char* sprite_name, Sprite* sprite);

	void loadSprite(const char* sprite);
	void loadSprite(Sprite* sprite);

	void unloadSprite(const char* sprite);
	void unloadSprite(Sprite* sprite);

	void handleSprites();

	Sprite* getSprite(const char* sprite_name);
};