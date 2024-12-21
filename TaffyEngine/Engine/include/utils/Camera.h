#pragma once

struct Camera final{
private:
	float x = 0, y = 0, zoom = 1;
	bool activated = true;
public:
	static Camera default_camera;

	Camera();
	Camera(float x, float y);
	Camera(float x, float y, float z);
	void setPos(float x, float y);
	void activate();
	void deactivate();
	void setZoom(float zoom);

	float getX();
	float getY();
	float getZoom();
	bool isActivated();
};
