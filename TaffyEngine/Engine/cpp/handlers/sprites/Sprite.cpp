#include "../../../include/handlers/sprites/Sprites.h"

Sprite::Sprite(const char* sprite) {
	sprite_sheet = sprite;
	ci = CropInfo(0, 0, -1, -1);
	loaded = false;
}
Sprite::Sprite(const char* sheet, CropInfo ci) {
	sprite_sheet = sheet;
	this->ci = ci;
	loaded = false;
}

void Sprite::load() {
	if (loaded) unload();
	loaded = true;
	new (&sprite_image) Image(sprite_sheet, ci);
}
void Sprite::unload() {
	if (!loaded) { return; }
	sprite_image.~Image();
	loaded = false;
}

float Sprite::getW() {
	return sprite_image.w;
}
float Sprite::getH() {
	return sprite_image.h;
}

bool Sprite::isLoaded() {
	return loaded;
}

Image& Sprite::getImage() {
	return sprite_image;
}