#pragma once
#include "../../utils/Utils.h"

struct Sprite {
	const char* sprite_sheet;
	CropInfo ci;

	Image sprite_image;
	bool loaded;

	Sprite(const char* sprite);
	Sprite(const char* sheet, CropInfo ci);

	void load();
	void unload();
};

