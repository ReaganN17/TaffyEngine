//gridObject requires a grid
//however initlized grid is not good because grid can change
//taking a grid as a parameter is the best i can do unfortunately

struct GridObject : Object {
	Grid* grid;
	u16 xG = 0, yG = 0, scale;
	u8 id = 2, overlapped = 0;
	//ids 
	//0 is empty space
	//1 is unbreakable wall
	//2 is player
	//3 - 10 signifies enemies and their power level
	//11-255 anything else

	GridObject();
	~GridObject();
	GridObject(Grid* grid, u16 x, u16 y, const char* sprite, float scale, zLayer z, u8 id);
	GridObject(Grid* grid, u16 x, u16 y, const char* sprite, u16 cx, u16 cy, u16 cw, u16 ch, float scale, zLayer z, u8 id);

	void setGridPos(float x, float y);
	void setGridVector(u8 dir, float mag);
	virtual bool checkValid(int x, int y);
};

#include "sentientGO.cpp"

//Obligatory constructor
GridObject::GridObject() {}

//obligatory destructor
GridObject::~GridObject() { if (!ob.instance) return; }


//constructor
GridObject::GridObject(Grid* grid, u16 x, u16 y, const char* sprite, u16 cx, u16 cy, u16 cw, u16 ch, float scale = 1, zLayer z = MIDDLE, u8 id = 2) : Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale * scale, grid->grid_scale* scale, sprite, z, cx, cy, cw, ch) {
	xG = x, yG = y, this->id = id;  this->grid = grid;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	overlapped = grid->grid[x + y * grid->gw];
	grid->grid[x + y * grid->gw] = id;
}

GridObject::GridObject(Grid* grid, u16 x, u16 y, const char* sprite, float scale = 1, zLayer z = MIDDLE, u8 id = 2) : Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale* scale, grid->grid_scale* scale, sprite, z) {
	xG = x, yG = y, this->id = id;  this->grid = grid;
	ob.cameraLinked = true;

	this->scale = scale * 1000;

	overlapped = grid->grid[x + y * grid->gw];
	grid->grid[x + y * grid->gw] = id;
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

bool GridObject::checkValid(int x, int y) {
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
}