//gridObject requires a grid
//however initlized grid is not good because grid can change
//taking a grid as a parameter is the best i can do unfortunately

struct GridObject : Object {
	Grid* grid;
	u16 xG = 0, yG = 0;
	u8 id = 2, overlapped = 0;
	

	//Obligatory constructor
	GridObject () {}

	//obligatory destructor
	~GridObject() { if (!ob.instance) return; }


	//constructor
	GridObject(Grid* grid, u16 x, u16 y, const char* sprite, float scale = 1, zLayer z = MIDDLE, u8 id = 2) : Object(grid->dx + x * grid->grid_scale, grid->dy - y * grid->grid_scale, grid->grid_scale * scale, grid->grid_scale * scale, sprite, z){
		xG = x, yG = y, this->id = id;  this->grid = grid;
		ob.cameraLinked = true;

		overlapped = grid->grid[x + y * grid->gw];
		grid->grid[x + y * grid->gw] = id;
	}

	GridObject& moveToPos(int x, int y) {
		if (checkValid(x, y)) {
			grid->grid[xG + yG * grid->gw] = overlapped;
			xG = x, yG = y;
			grid->grid[xG + yG * grid->gw] = id;

			setPos(grid->dx + xG * grid->grid_scale, grid->dy - yG * grid->grid_scale);
		}
		return *this;
	}

	virtual bool checkValid(int x, int y) {
		if (x < 0 || x >= grid->gw) return false;
		if (y < 0 || y >= grid->gh) return false;
		return true;
	}

	virtual GridObject& update() { return *this; }
};

#include "player.cpp"