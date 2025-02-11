#include "../../include/utils/Camera.h"

Camera Camera::default_camera;

Camera::Camera() {}

Camera::Camera(float x, float y) : x(x), y(y) {}

Camera::Camera(float x, float y, float zoom) : Camera(x, y) { setZoom(zoom); }

void Camera::setPos(float x, float y) {
	this->x = x;
	this->y = y;
}

void Camera::activate() {
	activated = true;
}

void Camera::deactivate() {
	activated = false;
}

void Camera::setZoom(float zoom) {
	if (zoom <= 0) return;
	this->zoom = zoom;
}

float Camera::getX() {
	return x;
}

float Camera::getY() {
	return y;
}

float Camera::getZoom() {
	return zoom;
}

bool Camera::isActivated() {
	return activated;
}