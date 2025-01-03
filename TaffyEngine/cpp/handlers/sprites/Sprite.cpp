#include "../../../include/handlers/sprites/Sprites.h"

Sprite::Sprite() { 
	sprite_sheet = ""; 
	ci = CropInfo(0, 0, -1, -1); 
}

Sprite::Sprite(const char* sprite) {
	sprite_sheet = sprite;
	ci = CropInfo(0, 0, -1, -1);
}
Sprite::Sprite(const char* sheet, CropInfo ci) {
	sprite_sheet = sheet;
	this->ci = ci;
}

Sprite::~Sprite() {
	unload();
}

void Sprite::load() {
	if (isLoaded()) unload();
	sprite_image.loadImage(sprite_sheet);
	sprite_image.crop(ci);
}
void Sprite::unload() {
	if (!isLoaded()) { return; }
	sprite_image.unloadImage();
}

float Sprite::getW() {
	return sprite_image.w;
}
float Sprite::getH() {
	return sprite_image.h;
}

bool Sprite::isLoaded() {
	return sprite_image.data != NULL;
}

Image& Sprite::getImage() {
	return sprite_image;
}

