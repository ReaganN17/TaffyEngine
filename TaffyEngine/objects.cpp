//Header

enum zLayer {
	FARBACK, BACK, MIDDLE, FRONT, FARFRONT
};

struct Object {
	int x = 0, y = 0, w = 0, h = 0;
	u8 alpha = 255;
	bool instance = false;

	u16 pos;

	u32 color = 0;

	zLayer z = MIDDLE;
	const char* file = "";

	Object();
	Object(int x, int y, zLayer z);
	Object(int x, int y, int w, int h, zLayer z);
	Object(int x, int y, const char* filename, zLayer z);
	Object(int x, int y, int w, int h, const char* filename,  zLayer z);
	Object(int x, int y, int w, int h, u32 c, u8 a, zLayer z);
	~Object();

	virtual void render();
	bool isImg();

	Object& velocity(float xInput, float yInput, float dt);
	Object& setPos(int x, int y);
	Object& setScale(int w, int h);
	Object& setAlpha(int val);

	Object& changeImg(const char* filename, int newW, int newH);
};

//End of Header

vector<Object*> Z0;
vector<Object*> Z1;
vector<Object*> Z2;
vector<Object*> Z3;
vector<Object*> Z4;

global_var vector<vector<Object*>*> objects = { &Z0, &Z1, &Z2, &Z3, &Z4 };

#include "uibuttons.cpp"
#include "maps.cpp"
#include "uiobjects.cpp"

//Class

#define addObject(layer)\
layer.push_back(&*this);\
pos = layer.size() - 1;\
instance = true;

//Constructors that shouldn't be directly used
Object::Object() { }

Object::Object(int x, int y, zLayer z = MIDDLE) :x(x), y(y), z(z) { 
	switch (z) {
		case FARBACK: { addObject(Z0); break; }
		case BACK: { addObject(Z1); break; }
		case MIDDLE: { addObject(Z2); break; }
		case FRONT: {addObject(Z3); break;}
		case FARFRONT: { addObject(Z4); break; }
	}
}

Object::Object(int x, int y, int w, int h, zLayer z = MIDDLE) :x(x), y(y), w(w),h(h), z(z) { 
	switch (z) {
		case FARBACK: { addObject(Z0); break; }
		case BACK: { addObject(Z1); break; }
		case MIDDLE: { addObject(Z2); break; }
		case FRONT: { addObject(Z3); break; }
		case FARFRONT: { addObject(Z4); break; }
	}
}

//3 Constructors that should be used
Object::Object(int x, int y, const char* filename, zLayer z = MIDDLE) :Object(x, y, z) {
	file = filename;
	w = getWidth(filename);
	h = getHeight(filename);
	alpha = 255;
}

Object::Object(int x, int y, int w, int h, const char* filename,  zLayer z = MIDDLE) :Object(x, y, w, h, z) {
	file = filename;
	alpha = 255;
}

Object::Object(int x, int y, int w, int h, u32 c, u8 a = 255, zLayer z = MIDDLE) :Object(x, y, w, h, z) {
	color = c;
	alpha = a;
}

//stupid shitty vectors are sensitive gotta do everything exact
#define cleanUp(layer)\
auto clean = layer.begin() + pos;\
layer.erase(clean);\
for (auto i : layer) {\
if (i->pos > this->pos) {i->pos--;}\
}\



Object::~Object() {
	//WHY DOES THIS FUCKING FIX IT
	if (!instance) return;

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
		renderImageV2(file, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom, h * mainCam.zoom, alpha);
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

Object& Object::setPos(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

Object& Object::setScale(int w, int h) {
	this->w = h;
	this->h = h;
	return *this;
}

Object& Object::setAlpha(int val) {
	alpha = clamp(0, val, 255);

	return *this;
}

Object& Object::changeImg(const char* filename, int newW, int newH) {
	file = filename;
	w = (newW < 0) ? getWidth(filename) : newW;
	h = (newH < 0) ? getHeight(filename) : newH;
	return *this;
}

//End of Class functions

//Render Objects


internal void renderAllObjects() {
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		for (auto it1 = (*it)->begin(); it1 != (*it)->end(); ++it1) {
			(*it1)->render();
		}
	}
}

internal void destroyAllObjects(){
	//do it twice cause first one aint guarenteed (jank ass programming)
	for (u8 i = 0; i < 2; i++) {
		for (auto it : objects) {
			for (auto it1 : *it) {
				(it1)->~Object();
			}
		}
	}
}



