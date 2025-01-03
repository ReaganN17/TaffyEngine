#pragma once


#include "Sprites.h"

class TextSprite : public Sprite {
	static std::map<char, Image> character_map;

	std::string text;
	u32 text_color = 0xffffffff, highlight_color = 0x00000000;

	void load();

	void unload();
public:
	TextSprite();

	TextSprite(std::string text);

	TextSprite(std::string text, u32 text_color, u32 highlight_color);
};