

struct Player : GridObject {

	//obligatory constructor
	Player() {}

	//obligatory destructor
	~Player() { if (!ob.instance) return;}

	//actual constructor
	Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z = MIDDLE, u8 id = 2) : GridObject(grid, x, y, file, scale, z, id) {}

	void move(PControls inputs) {
		if (pressed(inputs.up)) { movement(xG, yG - 1); }
		if (pressed(inputs.down)) { movement(xG, yG + 1); }
		if (pressed(inputs.left)) { movement(xG - 1, yG); }
		if (pressed(inputs.right)) { movement(xG + 1, yG); }
	}

	Player& movement(int x, int y) {
		if (checkValid(x, y)) {
			grid->grid[xG + yG * grid->gw] = overlapped;
			xG = x, yG = y;
			grid->grid[xG + yG * grid->gw] = id;

			setPos(grid->dx + xG * grid->grid_scale, grid->dy - yG * grid->grid_scale);
		}
		return *this;
	}

	bool checkValid(int x, int y) {
		if (x < 0 || x >= grid->gw) return false;
		if (y < 0 || y >= grid->gh) return false;
		if (grid->grid[x + y * grid->gw] != 0) return false;
		return true;
	}


};