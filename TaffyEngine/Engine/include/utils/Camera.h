#pragma once

/*
Camera Class

Changes coordinate positions and scale on the render screen
*/
class Camera final{
private:
	float x = 0, y = 0, zoom = 1;
	bool activated = true;
public:
	//Default Camera
	static Camera default_camera;

	//Default Construtor
	Camera();

	/**
	* Point Constructor
	* 
	* @param x, y coordinate
	*/
	Camera(float x, float y);

	/**
	* Point amd zoom Constructor
	*
	* @param x, y coordinate
	* @param zoom > 0
	*/
	Camera(float x, float y, float z);

	/**
	* Sets the position of the camera
	* 
	* @param x, y coordinate
	*/
	void setPos(float x, float y);

	//Activates the Camera; any object connected to it will render
	void activate();

	//Deactivates the Camera; any object connected to it will not render
	void deactivate();

	/**
	* Sets the zoom of the camera
	* 
	* @param Zoom > 0
	*/
	void setZoom(float zoom);

	//@return X position
	float getX();

	//@return Y position
	float getY();

	//@return Zoom scale
	float getZoom();

	//@return Activation status
	bool isActivated();
};
