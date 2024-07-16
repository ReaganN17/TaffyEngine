//Header

union ObjectByte
{
	unsigned char byte = 0;

	struct
	{
		u8 layer : 4;
		bool spriteSheet : 1;
		bool instance : 1;
		bool renderGroup : 1;
		bool cameraLinked : 1;
	};
};

struct Object {
	Image* spritesheet;
	CropInfo crop;
	Image sprite;
	int x = 0, y = 0, w = 0, h = 0;
	u32 color = 0;
	u8 shade = 0;
	ObjectByte ob;

	Object();
	Object(int x, int y);
	Object(int x, int y, int w, int h);
	Object(int x, int y, const char* filename, u8 z);
	Object(int x, int y, int w, int h, const char* filename, u8 z);

	//spritesheet option
	Object(int x, int y, u8 z, Image* spritesheet, CropInfo crop);
	Object(int x, int y, int w, int h, u8 z, Image* spritesheet, CropInfo crop);


	Object(int x, int y, int w, int h, u32 c, u8 z);
	~Object();

	virtual void render();
	virtual Object& update();

	virtual void destroyObject();

	void addObject(u8 z);
	void removeObject();

	bool isImg();
	virtual bool collide(Object* obj);

	Object& animateToPos(int x, int y, float time);

	Object& setPos(int x, int y);
	Object& setScale(int w, int h);
	Object& setShade(u8 shade);
	Object& changeImg(const char* filename);
	Object& changeCrop(CropInfo crop);


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

#include "basicEvents.cpp"

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
internal void destroyAllObjects() {
	for (auto it: objects) {


		for (auto it1 : (*it)) {
			(it1)->~Object();
		}

		(*it).clear();
	}
}

//updates all objects in a z layer
internal void updateZObjects(vector<Object*>* z) {
	for (auto o : *z) {
		(o)->update();
	}
}

//updates all objects in the render group
internal void updateAllObjects() {
	for (auto i : objects) {
		updateZObjects(i);
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
	w = sprite.w;
	h = sprite.h;
	addObject(z);
	ob.cameraLinked = true;
	ob.instance = true;
}

//advanced object image constructor (basic but with width and height)
Object::Object(int x, int y, int w, int h, const char* filename, u8 z) : Object(x, y, w, h) {
	sprite.create(filename);
	addObject(z);
	ob.cameraLinked = true;
	ob.instance = true;
}

//basic object constructor from spritesheet
Object::Object(int x, int y, u8 z, Image* spritesheet, CropInfo crop) : Object(x, y) {
	new (&sprite) Image(spritesheet->produceCrop(crop.cx, crop.cy, crop.cw, crop.ch));
	this->crop = crop;
	this->spritesheet = spritesheet;
	w = sprite.w;
	h = sprite.h;
	addObject(z);
	ob.instance = true;
	ob.cameraLinked = true;
	ob.spriteSheet = true;
}

//advanced object constructor from spritesheet
Object::Object(int x, int y, int w, int h, u8 z, Image* spritesheet, CropInfo crop) : Object(x, y, w, h) {
	new (&sprite) Image(spritesheet->produceCrop(crop.cx, crop.cy, crop.cw, crop.ch));
	this->crop = crop;
	this->spritesheet = spritesheet;
	addObject(z);
	ob.instance = true;
	ob.cameraLinked = true;
	ob.spriteSheet = true;
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

	sprite.~Image();

	ob.instance = false;
}

//removes from render group then destroys
void Object::destroyObject() {
	removeObject();
	Object::~Object();
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
}

//remove from z layer
void Object::cleanLayer(vector<Object*>* z) {
	auto i = find(z->begin(), z->end(), &*this);
	if (i != z->end()) {
		z->erase(i);
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
Object& Object::update() { return *this; }

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

ObjectMove moveVar;
Object& Object::animateToPos(int x, int y, float time) {

	moveVar.init(&*this, x, y, time);

	return *this;
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
Object& Object::changeCrop(CropInfo crop) {
	if (!ob.spriteSheet) return *this;

	sprite.~Image();
	this->crop = crop;
	new (&sprite) Image(spritesheet->produceCrop(crop.cx, crop.cy, crop.cw, crop.ch));

	return *this;
}

//End of Class functions





