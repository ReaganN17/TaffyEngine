//Header

union ObjectByte
{
	unsigned char byte = 0;

	struct
	{
		u8 layer : 5;
		bool instance : 1;
		bool renderGroup : 1;
		bool cameraLinked : 1;
	};
};

struct Object {
	Image sprite;
	int x = 0, y = 0, w = 0, h = 0;
	u32 color = 0;
	u16 pos;
	u8 shade = 0;
	ObjectByte ob;

	Object();
	Object(int x, int y);
	Object(int x, int y, int w, int h);
	Object(int x, int y, const char* filename, u8 z);
	Object(int x, int y, int w, int h, const char* filename, u8 z);
	Object(int x, int y, int w, int h, const char* filename, u8 z, u16 cx, u16 cy, u16 cw, u16 ch);
	Object(int x, int y, int w, int h, u32 c, u8 z);
	~Object();

	virtual void render();
	virtual Object& update(float dt);

	void addObject(u8 z);
	void removeObject();

	bool isImg();
	virtual bool collide(Object* obj);

	Object& setPos(int x, int y);
	Object& setScale(int w, int h);
	Object& setShade(u8 shade);
	Object& changeImg(const char* filename);
	Object& changeCrop(const char* filename, u16 cx, u16 cy, u16 cw, u16 ch);


private:
	void addLayer(vector<Object*>* z);
	void cleanLayer(vector<Object*>* z);
};

//End of Header

vector<Object*> FarBack_0;
vector<Object*> Back_1;
vector<Object*> Middle_2;
vector<Object*> Front_3;
vector<Object*> FarFront_4;

global_var vector<vector<Object*>*> objects = { &FarBack_0, &Back_1, &Middle_2, &Front_3, &FarFront_4 };

#include "uibuttons.cpp"
#include "maps.cpp"
#include "uiobjects.cpp"

//Render Objects

//renders all objects in a z layer
internal void renderZObjects(vector<Object*>* z) {
	for (auto o : *z) {
		(o)->render();
	}
}

//rendres all objects in the render group
internal void renderAllObjects() {
	for (auto i : objects) {
		renderZObjects(i);
	}
}

//destroys all objets in the render group
//done backwards due to how remove object function works
internal void destroyAllObjects() {
	for (auto it: objects) {
		//this is stupid
		reverse((it)->begin(), (it)->end());
		for (auto it1 : (*it)) {
			(it1)->~Object();
		}
	}
}

//updates all objects in a z layer
internal void updateZObjects(vector<Object*>* z, float dt) {
	for (auto o : *z) {
		(o)->update(dt);
	}
}

//updates all objects in the render group
internal void updateAllObjects(float dt) {
	for (auto i : objects) {
		updateZObjects(i, dt);
	}
}

//Class

//default constructor
Object::Object() {}

//object point constructor
Object::Object(int x, int y) : x(x), y(y) {}

//object rectangle constructor
Object::Object(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

//basic object image constructor
Object::Object(int x, int y, const char* filename, u8 z) :Object(x, y) {
	sprite.create(filename);
	addObject(z);
	ob.instance = true;
}

//advanced object image constructor (basic but with width and height)
Object::Object(int x, int y, int w, int h, const char* filename, u8 z) : Object(x, y, w, h) {
	sprite.create(filename);
	addObject(z);
	ob.instance = true;
}

//advance object image construct when using sprite sheets
Object::Object(int x, int y, int w, int h, const char* filename, u8 z, u16 cx, u16 cy, u16 cw, u16 ch) : Object(x, y, w, h, filename, z) {
	sprite.create(filename);
	sprite.crop(cx, cy, cw, ch);
}

//object colored rectangle constructor
Object::Object(int x, int y, int w, int h, u32 color, u8 z) : Object(x, y, w, h) {
	this->color = color;
	addObject(z);
	ob.instance = true;
}

//decontructs object if it exists
Object::~Object() {
	if (!ob.instance) return;

	removeObject();

	ob.instance = false;
}

//adds object to render group based on z layer
//if already in render group, removes first
void Object::addObject(u8 z) {
	if (ob.renderGroup) return;

	ob.layer = z;

	switch (z) {
	case 0: addLayer(&FarBack_0); break;
	case 1: addLayer(&Back_1); break;
	case 2: addLayer(&Middle_2); break;
	case 3: addLayer(&Front_3); break;
	case 4: addLayer(&FarFront_4); break;
	}

	ob.renderGroup = true;
}

//removes object render group, if already moved dont
void Object::removeObject() {
	if (!ob.renderGroup) return;

	switch (ob.layer) {
	case 0: cleanLayer(&FarBack_0); break;
	case 1: cleanLayer(&Back_1); break;
	case 2: cleanLayer(&Middle_2); break;
	case 3: cleanLayer(&Front_3); break;
	case 4: cleanLayer(&FarFront_4); break;
	}

	ob.renderGroup = false;
}

//add to a z layer
void Object::addLayer(vector<Object*>* z) {
	z->push_back(&*this);
	pos = z->size() - 1;
}

//remove from z layer
void Object::cleanLayer(vector<Object*>* z) {
	auto clean = z->begin() + pos;
	z->erase(clean);
	for (auto i : *z) {
		if (i->pos > this->pos) { i->pos--; }
	}
}

//render
void Object::render() {
	switch (ob.cameraLinked | (isImg() << 1)) {
	case 0: renderRect(x, y, w * 0.5, h * 0.5, color); break;
	case 1:	renderRect((x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * 0.5 * mainCam.zoom, h * 0.5 * mainCam.zoom, color); break;
	case 2: renderImageV2(&sprite, x, y, w, h, shade); break;
	case 3: renderImageV2(&sprite, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom, h * mainCam.zoom, shade); break;
	}
}

//update loop
Object& Object::update(float dt) { return *this; }

//image or rectangle
bool Object::isImg() { return sprite.data != NULL; }

//collision (rect only)
bool Object::collide(Object* obj) {
	if (obj->x + 0.5 * obj->w > this->x - 0.5 * this->w) return true;
	if (obj->x - 0.5 * obj->w < this->x + 0.5 * this->w) return true;
	if (obj->y + 0.5 * obj->h > this->y - 0.5 * this->h) return true;
	if (obj->y - 0.5 * obj->h < this->y + 0.5 * this->h) return true;
	return false;
}

//sets x and y pos
Object& Object::setPos(int x, int y) { 

	this->x = x; this->y = y; return *this;
}

//sets scale
Object& Object::setScale(int w, int h) {

	this->w = w; this->h = h; return *this;
}

//sets shade
Object& Object::setShade(u8 shade) {
	this->shade = shade; return*this;
}

//retains height and width
Object& Object::changeImg(const char* filename) {
	sprite.create(filename);

	return *this;
}

//changes image on the sprite sheet
Object& Object::changeCrop(const char * filename, u16 cx, u16 cy, u16 cw, u16 ch) {
	sprite.create(filename);
	sprite.crop(cx, cy, cw, ch);

	return *this;
}

//End of Class functions





