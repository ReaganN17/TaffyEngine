#include "../../include/utils/Utils.h"

Camera::Camera() {}

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