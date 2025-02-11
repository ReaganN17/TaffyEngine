#include "../../../include/handlers/objects/Object.h"
#include "../../../include/handlers/ObjectHandler.h"
#include "../../../include/handlers/SpriteHandler.h"
#include "../../../include/base/Win32Render.h"

Object::Object() {
	x = 0;
	y = 0;
}

Object::Object(float x, float y, zLayer z) : x(x), y(y), z_layer(z) {
	ObjectHandler::addObject(this);
	instance = true;
}
Object::Object(float x, float y, float w, float h, zLayer z) : Object(x, y, z) {
	this->w = max(w, 0);
	this->h = max(h, 0);
}

Object::Object(float x, float y, const char* sprite, zLayer z) : Object(x, y, z) {
	this->sprite = SpriteHandler::getSprite(sprite);
	SpriteHandler::loadSprite(sprite);

	this->w = (sprite != nullptr) ? this->sprite->getW() : 0;
	this->h = (sprite != nullptr) ? this->sprite->getH() : 0;
}
Object::Object(float x, float y, float w, float h, const char* sprite, zLayer z) : Object(x, y, z) {
	this->sprite = SpriteHandler::getSprite(sprite);
	SpriteHandler::loadSprite(sprite);

	this->w = w;
	this->h = h;
}

Object::Object(float x, float y, float w, float h, u32 c, zLayer z) : Object(x, y, z) {
	this->color = c;
	this->w = w;
	this->h = h;
}

Object::~Object() {
	ObjectHandler::removeObject(this);
	instance = false;
}

void Object::render() {
	if (hidden || !camera->isActivated()) return;

	if (sprite != nullptr) {
		Win32Render::renderImage(sprite->getImage(), 
			(x - camera->getX()) * camera->getZoom(),
			(y - camera->getY()) * camera->getZoom(), 
			w * camera->getZoom(), 
			h * camera->getZoom(), 
			shader);
	}
	else {
		Win32Render::renderRect(
			(x - camera->getX()) * camera->getZoom(),
			(y - camera->getY()) * camera->getZoom(),
			w * camera->getZoom(),
			h * camera->getZoom(),
			color, shader);
	}
}
void Object::update() {
	periodic();
}

void Object::periodic() {

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

void Object::hide(bool hide) { hidden = hide; }

void Object::setShader(Shader shader) {
	setOpacity(shader.opacity);
	setShade(shader.shade_color, shader.shade_scale);
}

void Object::setShade(u32 color, u8 scale) { shader.shade_color = color; shader.shade_scale = scale; }
void Object::setOpacity(u8 opac) { shader.opacity = opac; }
void Object::connectCamera(Camera& camera) { this->camera = &camera; }

Sprite* Object::getSprite() { return sprite; }
void Object::setSprite(const char* sprite_name, bool sprite_dimensions) {
	this->sprite = SpriteHandler::getSprite(sprite_name);
	SpriteHandler::loadSprite(sprite_name);

	if (sprite_dimensions) {
		w = (sprite != nullptr) ? this->sprite->getW() : 0;
		h = (sprite != nullptr) ? this->sprite->getH() : 0;
	}
}