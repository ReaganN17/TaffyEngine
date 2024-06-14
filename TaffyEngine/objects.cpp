//Header

enum zLayer {
	FARBACK, BACK, MIDDLE, FRONT, FARFRONT
};

struct Object {
	float x = 0, y = 0, w = 0, h = 0, alpha = 1;
	bool instance = false;

	list<Object*>::iterator itr;

	u32 color = 0;

	zLayer z = MIDDLE;
	const char* file = "";

	Object();
	Object(float x, float y, zLayer z);
	Object(float x, float y, float w, float h, zLayer z);
	Object(float x, float y, const char* filename, zLayer z);
	Object(float x, float y, float w, float h, const char* filename,  zLayer z);
	Object(float x, float y, float w, float h, u32 c, float a, zLayer z);
	~Object();

	virtual void render();
	bool isImg();

	Object& velocity(float xInput, float yInput, float dt);
	Object& setPos(float x, float y);
	Object& setScale(float w, float h);
	Object& changeImg(const char* filename, float newW, float newH);
};

//End of Header

list<Object*> Z0;
list<Object*> Z1;
list<Object*> Z2;
list<Object*> Z3;
list<Object*> Z4;

global_var list<list<Object*>*> objects = { &Z0, &Z1, &Z2, &Z3, &Z4 };
//Class

#define addObject(layer)\
layer.push_back(&*this);\
itr = layer.begin();\
advance(itr, layer.size() - 1);\
instance = true;

//Constructors that shouldn't be directly used
Object::Object() { 
	addObject(Z2);
}

Object::Object(float x, float y, zLayer z = MIDDLE) :x(x), y(y), z(z) { 
	switch (z) {
		case FARBACK: { addObject(Z0); break; }
		case BACK: { addObject(Z1); break; }
		case MIDDLE: { addObject(Z2); break; }
		case FRONT: {addObject(Z3); break;}
		case FARFRONT: { addObject(Z4); break; }
	}
}

Object::Object(float x, float y, float w, float h, zLayer z = MIDDLE) :x(x), y(y), w(w),h(h), z(z) { 
	switch (z) {
		case FARBACK: { addObject(Z0); break; }
		case BACK: { addObject(Z1); break; }
		case MIDDLE: { addObject(Z2); break; }
		case FRONT: { addObject(Z3); break; }
		case FARFRONT: { addObject(Z4); break; }
	}
}

//3 Constructors that should be used
Object::Object(float x, float y, const char* filename, zLayer z = MIDDLE) :Object(x, y, z) {
	file = filename;
	w = getWidth(filename);
	h = getHeight(filename);
	alpha = 1;
}

Object::Object(float x, float y, float w, float h, const char* filename,  zLayer z = MIDDLE) :Object(x, y, w, h, z) {
	file = filename;
	alpha = 1;
}

Object::Object(float x, float y, float w, float h, u32 c, float a = 1, zLayer z = MIDDLE) :Object(x, y, w, h, z) {
	color = c;
	alpha = a;
}

#define cleanUp(layer)\
layer.erase(itr);\
for (list<Object*>::iterator it = itr; it != layer.end(); ++it) {\
	advance((*it)->itr, -1); \
}

Object::~Object() {
	switch (z) {
		case FARBACK: { cleanUp(Z0); } break;
		case BACK: { cleanUp(Z1); } break;
		case MIDDLE: { cleanUp(Z2); } break;
		case FRONT: { cleanUp(Z3); } break;
		case FARFRONT: { cleanUp(Z4); } break;
	}

	instance = false;
}

void Object::render() {
	if (isImg()) {
		renderImage(file, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom, h * mainCam.zoom, alpha);
	}
	else {
		renderRect((x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom / 2, h * mainCam.zoom / 2, color, alpha);
	}
}

bool Object::isImg() {
	return strlen(file) > 0;
}

Object& Object::velocity(float xInput, float yInput, float dt) {
	x += xInput * dt;
	y += yInput * dt;

	return *this;
}

Object& Object::setPos(float x, float y) {
	this->x = x;
	this->y = y;
	return *this;
}

Object& Object::setScale(float w, float h) {
	this->w = h;
	this->h = h;
	return *this;
}

Object& Object::changeImg(const char* filename, float newW, float newH) {
	file = filename;
	w = (newW < 0) ? getWidth(filename) : newW;
	h = (newH < 0) ? getHeight(filename) : newH;
	return *this;
}

//End of Class functions

//Render Objects
internal void renderAllObjects() {
	for (list<list<Object*>*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		for (list<Object*>::iterator it1 = (*it)->begin(); it1 != (*it)->end(); ++it1) {
			(*it1)->render();
		}
	}
}

