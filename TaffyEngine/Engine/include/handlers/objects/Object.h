#pragma once

#include "../../utils/Utils.h"
#include "../sprites/Sprites.h"

//Z value layers for the Object rendering
enum zLayer :u8 {
	Z_FARBACK, //Furthest Back Layer
	Z_BACK, //Back Layer
	Z_MIDDLE, //Middle Layer
	Z_FRONT, //Front Layer
	Z_FARFRONT, //Furthest Front Layer
	Z_NORENDER //No Render
};

/*
Object Class

Contains basic Object properties: position, sprite, shader
Either a rectangle, a sprite, or nothing
*/
class Object {
	friend ObjectHandler;

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
	//Default Constructor
	Object();

	/**
	* Point Constructor
	* 
	* @param x, y coordinate
	* @param z layer
	*/
	Object(float x, float y, zLayer z);

	/**
	* Rectangle Constructor
	* 
	* @param x, y coordinate
	* @param w, h dimension
	* @param z layer
	*/
	Object(float x, float y, float w, float h, zLayer z);

	/**
	* Sprite Constructor
	* 
	* @param x, y coordinate
	* @param sprite name
	* @param z layer
	*/
	Object(float x, float y, const char* sprite, zLayer z);

	/**
	* Sprite with dimensions Constructor
	*
	* @param x, y coordinate
	* @param w, h dimensions
	* @param sprite name
	* @param z layer
	*/
	Object(float x, float y, float w, float h, const char* sprite, zLayer z);

	/**
	* Rectangle Constructor
	*
	* @param x, y coordinate
	* @param w, h dimension
	* @param color in hex
	* @param z layer
	*/
	Object(float x, float y, float w, float h, u32 c, zLayer z);

	//Renders the object
	virtual void render();

	//Updates the object
	virtual void update();

	/**
	* Sets the x and y position of the object
	* 
	* @param x, y coordinate
	*/
	void setPos(float x, float y);

	/**
	* Sets the width and height of the object
	*
	* @param w, h, dimension
	*/
	void setSize(float w, float h);

	//@return X position
	float getX();

	//@return Y position
	float getY();

	//@return Width
	float getW();

	//@return Height
	float getH();

	//@param Sets hidden status of object
	void hide(bool hide);

	/**
	* Sets the Shader struct of the Object
	* 
	* @param Shader struct
	*/
	void setShader(Shader shader);

	/**
	* Sets the shade of the Object
	* 
	* @param Color of shade in hex
	* @param 0 - 255 Shade scale
	*/
	void setShade(u32 color, u8 scale);

	/**
	* Sets the opacity of the Obhect
	*
	* @param 0 - 255 opacity value
	*/
	void setOpacity(u8 opac);

	/**
	* Sets the object to a Camera
	* 
	* @param Camera reference
	*/
	void connectCamera(Camera& camera);

	/**
	* Gets the Sprite Class of the Object
	* 
	* @return Sprite pointer
	*/
	Sprite* getSprite();

	/**
	* Set the Sprite of the Object
	* 
	* @param Sprite name
	* @param change dimensions to raw Sprite dimensions
	*/
	void setSprite(const char* sprite_name, bool sprite_dimensions = false);
};