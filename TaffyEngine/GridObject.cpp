enum movementSeq {
	SELECTION,
	MOVEINIT, MOVE,
	MOVEINTERUPT, BOUNCEMOVE,
	MOVEEND
};

union MovementByte {
	u8 byte = 0;

	struct {
		u8 sequence : 6;
		u8 direction : 2;
	};
};

union Instance {
	u8 byte = 0;

	union {
		u8 space : 6;
		bool stateChange : 1;
		bool healthChange : 1;
	};


};

enum GOType {
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
	
	//moving purposes
	float pos;
	float acc;
	float rate = 0;
	float posborder;
	u8 distance;
	MovementByte mmb;

	//characteristics
	short health;
	u8 state;
	u8 power;
	u8 range;
	u8 speed;

	Instance inst;

	GridObject();
	~GridObject();
	GridObject(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, float scale, u8 id, u8 type);
	GridObject(Grid* grid, u16 x, u16 y, zLayer z, Image* spritesheet, CropInfo crop, float scale, u8 id, u8 type);
	
	

	virtual GridObject& move(float kickback);

	//moving functions
	virtual GridObject& moveinit();
	virtual GridObject& moveinteruptinit();
	virtual GridObject& bounceinit(float kickback);
	virtual GridObject& moveend();

	virtual GridObject& takeDamage(short base);
	virtual GridObject& toPos(int x, int y);

	virtual bool checkValidVector(u8 dir, float mag);
	virtual bool checkValid(int x, int y);

	void setGridPos(float x, float y);
	void setGridVector(u8 dir, float mag);

	void destroyObject();
	
	Grid*& removeCurID();
	Grid*& addCurID();
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

#include "player.cpp"
#include "enemy.cpp"

//Obligatory constructor
GridObject::GridObject() {}

//obligatory destructor
GridObject::~GridObject() { 
	if (!ob.instance) return; 
}

//constructor
GridObject::GridObject(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, float scale, u8 id, u8 type)
	: Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale* scale, grid->grid_scale* scale, sprite, z) 

{
	xG = x, yG = y, this->id = id;  this->grid = grid, this->type = type;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	grid->nodes[x + y * grid->gw].occupants.push_back(id);

	GridObjects.push_back(&*this);
}

GridObject::GridObject(Grid* grid, u16 x, u16 y, zLayer z, Image* spritesheet, CropInfo crop, float scale, u8 id, u8 type)
	: Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale* scale, grid->grid_scale* scale, z, spritesheet, crop)

{
	xG = x, yG = y, this->id = id;  this->grid = grid, this->type = type;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	grid->nodes[x + y * grid->gw].occupants.push_back(id);

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

	}

	/*

	switch (mmb.sequence) {
	case SELECTION: {

	} break;

	case MOVEINIT: {
		moveinit();
		mmb.sequence = MOVE;
	}

	case MOVE: {
		if (!checkValidVector(mmb.direction, 1)) {moveinteruptinit(); mmb.sequence = MOVEINTERUPT; break; }

		//kinematics (sorta)
		pos += rate * dt + acc * dt * dt * 0.5f;
		rate += acc * dt;



		switch (mmb.direction) {
		case MLEFT:
			if (xG - floor(pos) != xG) { removeCurID(); xG -= floor(pos); addCurID(); }
			else { xG -= floor(pos); }
			break;
		case MRIGHT:
			if (xG + floor(pos) != xG) { removeCurID(); xG += floor(pos); addCurID(); }
			else { xG += floor(pos); }
			break;
		case MUP:
			if (yG - floor(pos) != yG) { removeCurID(); yG -= floor(pos); addCurID(); }
			else { yG -= floor(pos); }
			break;
		case MDOWN:
			if (yG + floor(pos) != yG) { removeCurID(); yG += floor(pos); addCurID(); }
			else { yG += floor(pos); }
			break;
		}

		pos = fmod(pos, 1);

		setGridVector(mmb.direction, pos - 0.5);

		if (rate <= 0) { mmb.sequence = MOVEEND; }
	} break;

	case MOVEINTERUPT: {
		if (checkValidVector(mmb.direction, 1)) { mmb.sequence = MOVE; break; }

		pos += rate * dt + acc * dt * dt * 0.5f;
		rate += acc * dt;

		setGridVector(mmb.direction, pos - 0.5);
		if (pos >= posborder) {bounceinit(kickback); mmb.sequence = BOUNCEMOVE; setGridVector(mmb.direction, posborder - 0.5); break; }

		if (rate <= 0) { mmb.sequence = MOVEEND; }
	} break;


	case BOUNCEMOVE: {
		pos += rate * dt + acc * dt * dt * 0.5f;
		rate += acc * dt;

		setGridVector(mmb.direction, pos - 0.5);

		if (rate >= 0) { mmb.sequence = MOVEEND; }
	} break;

	case MOVEEND: {
		moveend();
		mmb.sequence = SELECTION;
	}break;
	}
	*/

	return *this;
}

//basic initialization
GridObject& GridObject::moveinit() {
	distance = range;
	pos = 0.5;

	rate = 2 * ((float)speed / 10.f) * distance;
	acc = -2 * distance * ((float)speed / 10.f) * ((float)speed / 10.f);

	return*this;
}

//interupt initlization
GridObject& GridObject::moveinteruptinit() {
	float b;

	switch (grid->getIDVector(mmb.direction, 1, xG, yG)) {
	case 1: b = 1; break;
	case 2: b = 0.65; break;
	case 3: b = 0.75; break;
	default: b = 1; break;
	}

	posborder = 0.5 * (2 - b - (float)scale / 1000) + 0.5;

	return *this;
}

//bounce initliztion
GridObject& GridObject::bounceinit(float kickback) {
	float factor = kickback;

	acc = 2 * (posborder - 0.5) / (factor * factor);
	rate = -2 * (posborder - 0.5) / (factor);

	pos = posborder;

	return *this;
}


//basic end
GridObject& GridObject::moveend() {
	rate = 0;
	acc = 0;
	setGridVector(mmb.direction, 0);

	return *this;
}

//basic take damage function
GridObject& GridObject::takeDamage(short base) {
	health -= base;

	return *this;
}

//basic teleportation (i dont even use this)
GridObject& GridObject::toPos(int x, int y) {
	removeCurID();
	xG = x, yG = y;
	addCurID();

	return *this;
}

//basic valid checkers for moving
bool GridObject::checkValidVector(u8 dir, float mag) {
	switch (dir) {
		case MLEFT: return checkValid(xG - mag, yG);
		case MRIGHT: return checkValid(xG + mag, yG);
		case MUP: return checkValid(xG, yG - mag);
		case MDOWN: return checkValid(xG, yG + mag);
	}
}

bool GridObject::checkValid(int x, int y) {
	if (grid->nodes[x + y * grid->gw].occupants.size() > 0) return false;
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
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


Grid*& GridObject::removeCurID() {
	grid->removeID(xG, yG, id);

	return grid;
}

Grid*& GridObject::addCurID() {
	grid->addID(xG, yG, id);

	return grid;

}

