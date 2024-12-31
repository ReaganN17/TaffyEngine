#pragma once
#include "Utils.h"

/*
Image Struct

Creates Image from files; PNG only
Contains bitmap data and image manipulations
*/
struct Image final {
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
	* Creates blank imae
	*
	* @param w, h dimension
	* @param channels
	*/
	Image(int w, int h, int channels);

	/**
	* Loads Image from other Image
	*
	* @param Image to copy
	*/
	Image(const Image& img);

	//Deconstructor
	~Image();

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
	bool isPNG(const char* filename);

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
};