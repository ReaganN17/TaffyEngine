#pragma once
#include "../../utils/Utils.h"

/*
Sprite Class

Contains properties of the sprite such as its image, file location, etc
*/
class Sprite final{
private:
	const char* sprite_sheet;
	CropInfo ci;

	Image sprite_image;
public:
	/**
	* Sprite Constructor
	* 
	* @param Sprite File
	*/
	Sprite(const char* sprite);

	/**
	* Sprite Constructor
	*
	* @param Sprite File
	* @param CropInfo
	*/
	Sprite(const char* sheet, CropInfo ci);

	//Deconstructor
	~Sprite();

	//Loads the Image bitmap into memory
	void load();

	//Unloads the Image bitmap from memory
	void unload();

	//@return Sprite Width
	float getW();

	//@return Sprite Height
	float getH();

	//@return If Image bitmap is loaded
	bool isLoaded();

	//@return Image reference
	Image& getImage();
};

