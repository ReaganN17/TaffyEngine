#pragma once

#include "../../utils/Utils.h"
#include "../sprites/Sprites.h"

enum zLayer :u8 {
	Z_FARBACK, 
	Z_BACK, 
	Z_MIDDLE, 
	Z_FRONT, 
	Z_FARFRONT, 
	Z_NORENDER
};

struct Object {

private:
	Sprite* sprite = nullptr;
	u32 color = 0x000000;
	zLayer z_layer = Z_NORENDER;
	bool hidden = false;
	bool instance = false;
	Shader shader;
	Camera* camera = &Camera::default_camera;

	float x = 0, y = 0, w = 0, h = 0;

public:
	Object();
	Object(float x, float y, zLayer z);
	Object(float x, float y, float w, float h, zLayer z);

	Object(float x, float y, const char* sprite, zLayer z);
	Object(float x, float y, float w, float h, const char* sprite, zLayer z);

	Object(float x, float y, float w, float h, u32 c, zLayer z);



	virtual void render();
	virtual void update();

	void setPos(float x, float y);
	void setSize(float w, float h);

	float getX();
	float getY();
	float getW();
	float getH();

	void hide(bool hide);
	void setShader(u8 opac, u32 color, u8 scale);
	void setShader(u32 color, u8 scale);
	void setShader(u8 opac);
	void connectCamera(Camera& camera);

	Sprite* getSprite();
	void setSprite(const char* sprite_name, bool sprite_dimensions = false);
};