#include "../../../include/TaffyEngine.h"

Object::Object() {
	x = 0;
	y = 0;
}

Object::Object(float x, float y, u8 z) : x(x), y(y) {
	ObjectHandler::getInstance().addObject(this, z);
	ob.layer = z;
	ob.sprite = false;
	ob.render = false;
	ob.instance = true;
	color = 0x000000;
	if (w < 0) w = 0;
	if (h < 0) h = 0;
}
Object::Object(float x, float y, float w, float h, u8 z) : Object(x, y, z) {
	this->w = w;
	this->h = h;
}

Object::Object(float x, float y, const char* sprite, u8 z) : x(x), y(y) {
	this->sprite = SpriteHandler::getInstance().getSprite(sprite);
	this->sprite->load();
	ObjectHandler::getInstance().addObject(this, z);
	ob.layer = z;
	ob.sprite = this->sprite != nullptr;
	ob.render = true;
	ob.instance = true;
	color = 0x000000;
	if (w < 0) w = (this->sprite != nullptr) ? this->sprite->sprite_image.w : 0;
	if (h < 0) h = (this->sprite != nullptr) ? this->sprite->sprite_image.h : 0;
}
Object::Object(float x, float y, float w, float h, const char* sprite, u8 z) : Object(x, y, sprite, z) {
	this->w = w;
	this->h = h;
}

Object::Object(float x, float y, float w, float h, u32 c, u8 z) : x(x), y(y), w(w), h(h), color(c) {
	ObjectHandler::getInstance().addObject(this, z);
	ob.layer = z;
	ob.sprite = false;
	ob.render = true;
	ob.instance = true;
}

Object::~Object() {
	sprite = nullptr;
	color = 0x000000;
	ob.byte = 0;
}

void Object::render() {
	if (ob.render) {
		if (ob.sprite) {
			Win32Render::renderImage(sprite->sprite_image, x, y, w, h, shader, camera);
		}
		else {
			Win32Render::renderRect(x, y, w, h, color, shader, camera);
		}
	}
}
void Object::update() {

}

void Object::setPos(float x, float y) {
	this->x = x, this->y = y;
}
void Object::setSize(float w, float h) {
	this->w = max(w, 0), this->h = max(h, 0);
}

float Object::getX() { return x; }
float Object::getY() { return y; }
float Object::getW() { return w; }
float Object::getH() { return h; }

void Object::hide(bool hide) { ob.render = hide; }
void Object::setShader(u8 opac, u32 color, u8 scale) { setShader(opac); setShader(color, scale); }
void Object::setShader(u32 color, u8 scale) { shader.shade_color = color; shader.shade_scale = scale; }
void Object::setShader(u8 opac) { shader.opacity = opac; }
void Object::connectCamera(Camera& camera) { this->camera = &camera; }

Sprite* Object::getSprite() { return sprite; }