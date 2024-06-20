

struct Player : GridObject {

	//obligatory constructor
	Player() {}

	//obligatory destructor
	~Player() { if (!ob.instance) return;}

	//actual constructor
	Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z = MIDDLE, u8 id = 2) : GridObject(grid, x, y, file, scale, z, id) {}

	Player& move(PControls inputs) {
		if (pressed(inputs.up)) {}
	}

	Player& movement(u16 x, u16 y) {

	}

	bool checkValid(u16 x, u16 y) {
		if (x < 0 || x >= grid->gw) return false;
		if (y < 0 || y >= grid->gh) return false;
		if (grid->grid[x + y * grid->gw] != 0) return false;
		return true;
	}


};