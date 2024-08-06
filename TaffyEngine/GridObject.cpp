enum movementSeq {
	SELECTION,
	MOVEINIT, MOVE,
	MOVEINTERUPT, BOUNCEMOVE,
	MOVEEND
};

union Instance {
	u8 byte = 0;

	struct {
		
		bool stateChange : 1;
		bool healthChange : 1;
		bool occupant : 1;
		u8 direction : 2;
		u8 space : 3;
	};


};

enum GOType : u8{
	CHARA, COLLECT, OBSTACLE
};

//gridObject requires a grid
//however initlized grid is not good because grid can change
//taking a grid as a parameter is the best i can do unfortunately
struct GridObject : Object {
	Grid* grid;
	Image* spriteSheet;
	u16 xG, yG, scale;
	u8 id, type;

	//characteristics
	short health;
	u8 state;
	u8 power;
	u8 range;
	u8 speed;

	Instance inst;

	GridObject();
	~GridObject();
	GridObject(Grid* grid, u16 x, u16 y, zLayer z, u32 color, float scale, u8 id, u8 type);
	GridObject(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, float scale, u8 id, u8 type);
	GridObject(Grid* grid, u16 x, u16 y, zLayer z, Image* spritesheet, CropInfo crop, float scale, u8 id, u8 type);

	virtual GridObject& move(float kickback);

	virtual GridObject& takeDamage(short base);

	virtual bool checkValidVector(u8 dir, float mag, initializer_list <u8> ids);
	virtual bool checkValid(int x, int y, initializer_list <u8> ids);

	void setGridPos(float x, float y);
	void setGridVector(u8 dir, float mag);

	void destroyObject();

	void addObj(u16 x, u16 y);
	void addObjCur();
	void removeObj();
};

vector<GridObject*> GridObjects;

GridObject* getObject(u16 x, u16 y, u8 id) {
	for (auto i : GridObjects) {
		if (i->xG == x && i->yG == y && i->id == id) { return i; }
	}

	return nullptr;
}

GridObject* getObjectVector(u8 dir, int mag, u16 x, u16 y, u8 id) {
	switch (dir) {
		case MLEFT: return getObject(x - mag, y, id);
		case MRIGHT:return getObject(x + mag, y, id);
		case MUP:return getObject(x, y - mag, id);
		case MDOWN:return getObject(x, y + mag, id);
	}

	return nullptr;
}

#include "commands.cpp"
#include "BasicWall.cpp"
#include "player.cpp"
#include "enemy.cpp"

//Obligatory constructor
GridObject::GridObject() {}

//obligatory destructor
GridObject::~GridObject() { 
	if (!ob.instance) return; 
}

GridObject::GridObject(Grid* grid, u16 x, u16 y, zLayer z, u32 color, float scale, u8 id, u8 type)
	: Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale* scale, grid->grid_scale* scale, color, z)
{
	x = min(x, grid->gw - 1), y = min(y, grid->gw - 1);
	xG = x, yG = y, this->id = id;  this->grid = grid, this->type = type;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	if (z != 5) {
		grid->nodes[x + y * grid->gw].occupants.push_back(this);
		inst.occupant = true;
	}

	GridObjects.push_back(&*this);
}

//constructor
GridObject::GridObject(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, float scale, u8 id, u8 type)
	: Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale* scale, grid->grid_scale* scale, sprite, z) 

{
	x = min(x, grid->gw - 1), y = min(y, grid->gh - 1);
	xG = x, yG = y, this->id = id;  this->grid = grid, this->type = type;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	if (z != 5) {
		grid->nodes[x + y * grid->gw].occupants.push_back(this);
		inst.occupant = true;
	}

	GridObjects.push_back(&*this);
}

GridObject::GridObject(Grid* grid, u16 x, u16 y, zLayer z, Image* spritesheet, CropInfo crop, float scale, u8 id, u8 type)
	: Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale* scale, grid->grid_scale* scale, z, spritesheet, crop)

{
	x = min(x, grid->gw - 1), y = min(y, grid->gh - 1);
	xG = x, yG = y, this->id = id;  this->grid = grid, this->type = type;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	if (z != 5) {
		grid->nodes[x + y * grid->gw].occupants.push_back(this);
		inst.occupant = true;
	}

	GridObjects.push_back(&*this);
}


void GridObject::destroyObject() {
	auto i = find(GridObjects.begin(), GridObjects.end(), &*this);
	if (i != GridObjects.end()) {GridObjects.erase(i);}
	Object::destroyObject();
}

//basic move sequence
//can customize by creating own switch statement in object's own move function
//parameters are dt for movement time and kickback for recovery time when hitting a wall or other object
GridObject& GridObject::move(float kickback) {
	//4 directional movement

	if (moveCommand == nullptr) {
		moveCommand = new GOMove(&moveCommand, this, kickback);
		moveCommand->start();
	}

	return *this;
}

//basic take damage function
GridObject& GridObject::takeDamage(short base) {
	health -= base;

	inst.healthChange = true;

	return *this;
}

//basic valid checkers for moving
bool GridObject::checkValidVector(u8 dir, float mag, initializer_list <u8> ids) {
	return grid->checkValidVec(dir, mag, ids, xG, yG);
}

bool GridObject::checkValid(int x, int y, initializer_list <u8> ids) {
	return grid->checkValid(x, y, ids);
}


void GridObject::setGridPos(float x, float y) {
	setPos(grid->dx + x * grid -> grid_scale, grid-> dy - y * grid->grid_scale);
}

void GridObject::setGridVector(u8 dir, float mag) {
	switch (dir) {
		case MLEFT:setGridPos(xG - mag, yG); break;
		case MRIGHT: setGridPos(xG + mag, yG); break;
		case MUP: setGridPos(xG, yG - mag); break;
		case MDOWN: setGridPos(xG, yG + mag); break;
	}
}

void GridObject::addObj(u16 x, u16 y) {
	if (inst.occupant) { return; }

	grid->addObj(x, y, this);

	inst.occupant = true;
}

void GridObject::addObjCur() {
	if (inst.occupant) { return; }

	grid->addObj(xG, yG, this);

	inst.occupant = true;
}

void GridObject::removeObj() {
	if (!inst.occupant) { return; }

	grid->removeObj(xG, yG, this);

	inst.occupant = false;
}

