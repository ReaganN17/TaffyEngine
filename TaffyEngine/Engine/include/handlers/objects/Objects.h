#pragma once

#include "../../utils/Utils.h"
#include "../sprites/Sprites.h"

union ObjectByte
{
	unsigned char byte = 0;

	struct
	{
		u8 layer : 3;
		bool render : 1;
		bool instance : 1;
		bool sprite : 1;
		bool camera : 1;

		bool space : 1;
	};
};

enum zLayer :u8 {
	FARBACK, BACK, MIDDLE, FRONT, FARFRONT, UNRENDER
};

struct Object {

private:
	Sprite* sprite = nullptr;
	u32 color = 0x000000;
	ObjectByte ob;
	Shader shader;

	float x, y, w = -1, h = -1;

public:
	Object();
	Object(float x, float y, u8 z);
	Object(float x, float y, float w, float h, u8 z);

	Object(float x, float y, const char* sprite, u8 z);
	Object(float x, float y, float w, float h, const char* sprite, u8 z);

	Object(float x, float y, float w, float h, u32 c, u8 z);

	virtual ~Object();

	virtual void render();
	virtual void update();

	void setPos(float x, float y);
	void setSize(float w, float h);

	float getX();
	float getY();
	float getW();
	float getH();

	void setCamera(bool cam);
	void hide(bool hide);
	void setShader(u8 opac, u32 color, u8 scale);
	void setShader(u32 color, u8 scale);
	void setShader(u8 opac);

	Sprite* getSprite();
};


