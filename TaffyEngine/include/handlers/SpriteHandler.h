#pragma once
#include "../utils/Utils.h"
#include "sprites/Sprites.h"

/*
SpriteHandler Class

Manages all the Sprites and their corresponding names
*/
class SpriteHandler final {
	friend class EngineBase;

private:
	static std::map<const char*, Sprite*> sprites;
	static Timer handle_frequency;

	//Handles the Sprite; makes unused Sprites unload
	static void handleSprites(long frequency);

	//Disables Construction
	SpriteHandler() = delete;

public:

	/**
	* Adds Sprite to be used
	* 
	* @param String name to be called by
	* @param Sprite to add
	*/
	static void addSprite(const char* sprite_name, Sprite* sprite);

	/**
	* Updates Sprite for objects using the sprite
	* 
	* @param String name to update
	* @param Update to
	*/
	static void updateSprite(const char* sprite_name, Sprite* sprite);

	/**
	* Loads the Sprite bitmap into memory
	* 
	* @param String name for the Sprite
	*/
	static void loadSprite(const char* sprite);

	/**
	* Loads the Sprite bitmap into memory
	*
	* @param Sprite pointer
	*/
	static void loadSprite(Sprite* sprite);

	/**
	* Unloads the Sprite bitmap from memory
	* 
	* @param String name for the sprite
	*/
	static void unloadSprite(const char* sprite);

	/**
	* Unloads the Sprite bitmap from memory
	*
	* @param Sprite pointer
	*/
	static void unloadSprite(Sprite* sprite);

	/**
	* Gets the Sprite from name
	* 
	* @param String name for the sprite
	* @return Sprite pointer
	*/
	static Sprite* getSprite(const char* sprite_name);
};