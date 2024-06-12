//Header

enum ObjectType {
	CHARACTER, PLAYER, UI_BUTTON, UI_TEXT, BG, DEFAULT,
};

enum zLayer {
	FARBACK, BACK, MIDDLE, FRONT, FARFRONT
};

struct Object {
	float x = 0, y = 0, w = 0, h = 0, alpha = 1;
	bool instance = false;

	list<Object*>::iterator itr;
	u32 color = 0;

	ObjectType type = DEFAULT;
	zLayer z = MIDDLE;
	Image img;

	Object();
	Object(float x, float y, zLayer z);
	Object(float x, float y, float w, float h, zLayer z);
	Object(float x, float y, const char* filename, ObjectType t, zLayer z);
	Object(float x, float y, float w, float h, const char* filename, ObjectType t, zLayer z);
	Object(float x, float y, float w, float h, u32 c, ObjectType t, float a, zLayer z);
	~Object();

	void render(Camera cam);
	bool isImg();

	Object& velocity(float xInput, float yInput, float dt);
	Object& setPos(float x, float y);
	Object& setScale(float w, float h);
	Object& changeImg(const char* filename, float newW, float newH);
};

internal void renderAllObjects(Camera cam);

//End of Header

global_var list<Object*> Z0;
global_var list<Object*> Z1;
global_var list<Object*> Z2;
global_var list<Object*> Z3;
global_var list<Object*> Z4;

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
Object::Object(float x, float y, const char* filename, ObjectType t, zLayer z = MIDDLE) :Object(x, y, z) {
	img.create(filename);
	w = img.w;
	h = img.h;
	type = t;
	alpha = 1;
}

Object::Object(float x, float y, float w, float h, const char* filename, ObjectType t, zLayer z = MIDDLE) :Object(x, y, w, h, z) {
	img.create(filename);
	alpha = 1;
	type = t;
}

Object::Object(float x, float y, float w, float h, u32 c, ObjectType t, float a = 1, zLayer z = MIDDLE) :Object(x, y, w, h, z) {
	color = c;
	alpha = a;
	type = t;
}

Object::~Object() {
	img.~Image();

	switch (z) {
		case FARBACK: { Z0.erase(itr); } break;
		case BACK: { Z1.erase(itr); } break;
		case MIDDLE: { Z2.erase(itr); } break;
		case FRONT: { Z3.erase(itr); } break;
		case FARFRONT: { Z4.erase(itr); } break;
	}

	instance = false;
}

void Object::render(Camera cam) {
	if (isImg()) {
		renderImage(img, x, y, w / img.w, h / img.h, cam, alpha);
	}
	else {
		renderRect(x, y, w / 2, h / 2, color, alpha, cam);
	}
}

bool Object::isImg() {
	return img.data != NULL;
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
	img.create(filename);
	w = (newW<0)?img.w:newW;
	h = (newH < 0) ? img.h : newH;
}

//End of Class functions

//Non specific object functions

internal void renderAllObjects(Camera cam) {
	for (list<Object*>::iterator it = Z0.begin(); it != Z0.end(); ++it) {(*it)->render(cam);}
	for (list<Object*>::iterator it = Z1.begin(); it != Z1.end(); ++it) {(*it)->render(cam);}
	for (list<Object*>::iterator it = Z2.begin(); it != Z2.end(); ++it) {(*it)->render(cam);}
	for (list<Object*>::iterator it = Z3.begin(); it != Z3.end(); ++it) {(*it)->render(cam);}
	for (list<Object*>::iterator it = Z4.begin(); it != Z4.end(); ++it) {(*it)->render(cam);}
}