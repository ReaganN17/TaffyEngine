#pragma once

struct Camera final{
private:
	float x = 0, y = 0, zoom = 0;
	bool activated = false;
public:
	Camera();
	void setPos(float x, float y);
	void activate();
	void deactivate();
	void setZoom(float zoom);

	float getX();
	float getY();
	float getZoom();
	bool isActivated();
};
