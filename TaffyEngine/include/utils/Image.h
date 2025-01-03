#pragma once
#include "Utils.h"

/*
Image Struct

Creates Image from files; PNG only
Contains bitmap data and image manipulations
*/
struct Image final {

	friend class Sprite;
	friend class TextSprite;

	u8* data = NULL;
	size_t size = 0;
	u16 w = 0, h = 0;
	u8 channels = 0;

	//Default Constructor
	Image();

	/**
	* Loads Image from file
	* 
	* @param File name to load
	*/
	Image(const char* filename);

	/**
	* Loads Image from file and crops it
	*
	* @param File name to load
	* @param Crop dimensions
	*/
	Image(const char* filename, CropInfo ci);

	/**
	* Creates blank image
	*
	* @param w, h dimension
	* @param channels
	*/
	Image(int w, int h, int channels);

	/**
	* Creates manual bitmap image
	*
	* @param w, h dimension
	* @param channels
	*/
	Image(int w, int h, int channels, u8* data);

	/**
	* Loads Image from other Image
	*
	* @param Image to copy
	*/
	Image(const Image& img);

	//Deconstructor
	~Image();
	
	/**
	* Creates blank image
	* 
	* @param w, h dimensions
	* @param channels
	*/
	void createEmpty(int w, int h, int channels);

	/**
	* Loads image from file
	* 
	* @param File name to load
	* @return if load successful
	*/
	bool loadImage(const char* filename);

	/**
	* Unloads image; frees data
	* 
	* @return if Image was already unloaded
	*/
	bool unloadImage();

	//@return If file is PNG
	static bool isPNG(const char* filename);

	/**
	* Crops an Image
	* 
	* @param x, y position
	* @param width, height dimension
	* @return reference to self
	*/
	Image& crop(u16 cx, u16 cy, u16 cw, u16 ch);

	/**
	* Crops an Image from CropInfo
	*
	* @param CropInfo
	* @return reference to self
	*/
	Image& crop(CropInfo ci);

	/**
	* Grayscales the Image, does not change channel numbers
	* 
	* @return reference to self
	*/
	Image& grayscale_avg();


	/**
	* Converts Image into different channels
	* Will grayscale if 3 or 4 to 1 or 2
	* 
	* @param channel number (1-4) to convert into
	* @return reference to self
	*/
	Image& convertChannels(u8 channels);

	/**
	* Converts Image to 4 channels and replaces hex codes
	* Hex codes in AARRGGBB format
	* 
	* @param Hex Code to replace
	* @param Hex Code for replacement
	* @return reference to self
	*/
	Image& replace(u32 to_replace, u32 replacement);

private:

	//Combines Images from this Top Right to other Top Left
	//Used for TextSprite purposes
	bool combineTR(Image& image);

	//Combines Images from this Bottom Left to other Top Left
	//Used for TextSprite purposes
	bool combineBL(Image& image);
};

