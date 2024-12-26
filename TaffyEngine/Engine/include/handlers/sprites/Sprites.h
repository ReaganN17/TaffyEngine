#pragma once
#include "../../utils/Utils.h"

struct Sprite {
private:
	const char* sprite_sheet;
	CropInfo ci;

	Image sprite_image;
	bool loaded = false;
public:
	Sprite(const char* sprite);
	Sprite(const char* sheet, CropInfo ci);

	void load();
	void unload();

	float getW();
	float getH();

	bool isLoaded();

	Image& getImage();
};

