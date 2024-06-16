enum MoveTypes {
	LINEAR, INQUAD, OUTQUAD, INOUTQUAD, INCUBE, OUTCUBE, INOUTCUBE,
};

struct Camera {
	float x, y, zoom, alpha;
	u32 color;
	bool grey = false;
	bool shade = false;

	Camera(float x, float y, float zoom);
	~Camera();

	Camera& setPos(float x, float y, float zoom);

	Camera& greyscale(bool b);
	Camera& shader(bool s, u32 color, float alpha);
};

Camera::Camera(float x, float y, float zoom): x(x), y(y), zoom(zoom) {}

Camera::~Camera() {}

Camera& Camera::setPos(float x, float y, float z) {
	this->x = x;
	this->y = y;
	zoom = max(0, z);
	return *this;
}

Camera& Camera::greyscale(bool b) {
	grey = b;
	return *this;
}

Camera& Camera::shader(bool s, u32 color, float alpha) {
	shade = s;
	this->color = color;
	this->alpha = alpha;

	return *this;
}